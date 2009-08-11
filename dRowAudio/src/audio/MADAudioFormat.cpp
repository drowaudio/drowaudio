#include "MADAudioFormat.h"

#define FPM_64BIT
#define OPT_ACCURACY
#include "MAD/mad.h"

#define madFixedToIEEEfloat(mad) float(jlimit(double(-1.f), double(1.f), (mad * double(1.0 / (1 << MAD_F_FRACBITS)))))

const int XING_FRAMES = 0x00000001L;
const int XING_BYTES = 0x00000002L;
const int XING_TOC = 0x00000004L;
const int XING_SCALE = 0x00000008L;

struct xing
{
	long flags;
	unsigned long frames;
	unsigned long bytes;
	unsigned char toc[100];
	long scale;
};

class MADAudioFormatReader : public AudioFormatReader
{
public:

	MADAudioFormatReader(InputStream* const sourceStream) :
		AudioFormatReader(sourceStream, T("MPEG Audio Decoder (MAD)"))
	{
		usesFloatingPointData = true;
		sampleRate = 0;

		_streamStartPosition = 0;
		_samplesLeftInBuffer = 0;

		// Skip over ID3v2 tag (if there is one)
		{
			unsigned char tag_[10];

			memset(&tag_, 0, sizeof(tag_));
			int bytesRead = input->read(&tag_, sizeof(tag_));

			if(bytesRead == sizeof(tag_) && tag_[0] == 'I' && tag_[1] == 'D' && tag_[2] == '3')
			{
				_streamStartPosition = sizeof(tag_);
				_streamStartPosition += (int(tag_[6]) << 21)
					| (int(tag_[7]) << 14)
					| (int(tag_[8]) << 7)
					| int(tag_[9]);
			}
		}

		input->setPosition(_streamStartPosition);

		mad_stream_init(&_madStream);
		mad_frame_init(&_madFrame);
		mad_synth_init(&_madSynth);

		if(scanHeader(&_madFrame.header, &_xing) != -1)
		{
			input->setPosition(_streamStartPosition);

			_streamSize = input->getTotalLength();
			_madSynth.pcm.length = 0;
			_madCurTime = mad_timer_zero;
			if(_xing.flags & XING_FRAMES)
			{
					_madTimeLength = _madFrame.header.duration;
					mad_timer_multiply(&_madTimeLength, _xing.frames);
			}
			else
			{
					// Estimate playing time from file size
					mad_timer_set(&_madTimeLength, 0, 1, _madFrame.header.bitrate / 8);
					mad_timer_multiply(&_madTimeLength, (long)_streamSize);
			}

			sampleRate = _madFrame.header.samplerate;
			lengthInSamples = int64(double(mad_timer_count(_madTimeLength, MAD_UNITS_MILLISECONDS)) / 1000.f * double(sampleRate) + .5f);
			bitsPerSample = 16;
			numChannels = MAD_NCHANNELS(&_madFrame.header);

			input->setPosition(_streamStartPosition);
		}
	}

	~MADAudioFormatReader()
	{
		mad_synth_finish(&_madSynth);
		mad_frame_finish(&_madFrame);
		mad_stream_finish(&_madStream);
	}

	bool readSamples (int** destSamples, int numDestChannels, int startOffsetInDestBuffer,
                      int64 startSampleInFile, int numSamples)
//	bool read (int** destSamples, int64 startSampleInFile, int numSamples)
	{
		if(startSampleInFile >= lengthInSamples)
			return true;

		if(startSampleInFile + numSamples > lengthInSamples)
			numSamples = lengthInSamples - startSampleInFile;

		if(startSampleInFile != _lastSample)
		{
			if(seek(startSampleInFile))
				_lastSample = startSampleInFile;
			else
			{
				lengthInSamples = _lastSample;
				return true;
			}
		}

		float *left = (float*)destSamples[0];
		float *right = (float*)destSamples[1];
		int numToRead_ = 0;

		while(numSamples > 0)
		{
			if(_samplesLeftInBuffer <= 0)
			{
				while(mad_frame_decode(&_madFrame, &_madStream) == -1)
				{
					if(MAD_RECOVERABLE(_madStream.error))
						continue;

					int shift = 0;

					if(_madStream.next_frame)
					{
						shift = _madStream.bufend - _madStream.next_frame;
						memmove(_buffer, _madStream.next_frame, (size_t)shift);
					}

					int64 bytes = input->read(_buffer + shift, sizeof(_buffer) - shift);
					if(!bytes)
					{
						lengthInSamples = _lastSample;
						_madSynth.pcm.length = 0;
						_samplesLeftInBuffer = 0;

						return true;
					}

					mad_stream_buffer(&_madStream, _buffer, (long)(shift + bytes));
				}

				mad_synth_frame(&_madSynth, &_madFrame);

				_samplesLeftInBuffer = _madSynth.pcm.length;
				_leftChannel = _madSynth.pcm.samples[0];
				_rightChannel = _madSynth.pcm.samples[1];

				mad_timer_add(&_madCurTime, _madFrame.header.duration);
			}

			numToRead_ = (_samplesLeftInBuffer <= numSamples) ? _samplesLeftInBuffer : numSamples;

			numSamples -= numToRead_;
			_samplesLeftInBuffer -= numToRead_;
			_lastSample += numToRead_;

			while (numToRead_--)
			{
				if(left)
					(*left++) = madFixedToIEEEfloat(_madSynth.pcm.channels == 1 ? *_leftChannel : *_leftChannel++);

				if(right)
					(*right++) = madFixedToIEEEfloat(_madSynth.pcm.channels == 1 ? *_leftChannel++ : *_rightChannel++);
			}
		}

		return true;
	}

private:

	bool seek(int64 sample)
	{
		if(!input)
			return false;

		int64 milliseconds_ = mad_timer_count(_madTimeLength, MAD_UNITS_MILLISECONDS);
		int64 currentMillisecond_ = int64(double(sample) / double(sampleRate) * 1000);

		double fraction_ = double(currentMillisecond_) / double(milliseconds_);

		mad_timer_set(&_madCurTime, currentMillisecond_, currentMillisecond_, MAD_UNITS_MILLISECONDS);

		if(_xing.flags & XING_TOC)
		{
			int percent_, p1_, p2_;

			percent_ = int(fraction_ * 100);
			p1_ = (percent_ < 100) ? _xing.toc[percent_    ] : 0x100;
			p2_ = (percent_ <  99) ? _xing.toc[percent_ + 1] : 0x100;

			fraction_ = (p1_ + (p2_ - p1_) * (fraction_ * 100 - percent_)) / 0x100;
		}

		int64 pos_ = int64(double(_streamSize) * fraction_);
		input->setPosition(pos_ ? pos_ : _streamStartPosition);
		int size = input->read(_buffer, sizeof(_buffer));

		if(!size)
		{
			_madSynth.pcm.length = 0;
			_samplesLeftInBuffer = 0;

			return false;
		}

		mad_stream_buffer(&_madStream, _buffer, size);

		mad_frame_mute(&_madFrame);
		mad_synth_mute(&_madSynth);

		int skip_ = 2;

		do
		{
			if(!mad_frame_decode(&_madFrame, &_madStream))
			{
				mad_timer_add(&_madCurTime, _madFrame.header.duration);

				if(!(--skip_))
					mad_synth_frame(&_madSynth, &_madFrame);
			}
			else
				if(!MAD_RECOVERABLE(_madStream.error))
					break;
		}
		while(skip_);

		_madSynth.pcm.length = 0;
		_samplesLeftInBuffer = 0;

		return true;
	}

	int parseXing(struct xing *xing, struct mad_bitptr ptr, unsigned int bitlen)
	{
		if (bitlen < 64 || mad_bit_read(&ptr, 32) != (('X' << 24) | ('i' << 16) | ('n' << 8) | 'g'))
		{
			xing->flags = 0;
			return -1;
		}

		xing->flags = mad_bit_read(&ptr, 32);
		bitlen -= 64;

		if (xing->flags & XING_FRAMES)
		{
			if (bitlen < 32)
			{
				xing->flags = 0;
				return -1;
			}

			xing->frames = mad_bit_read(&ptr, 32);
			bitlen -= 32;
		}

		if (xing->flags & XING_BYTES) {
			if (bitlen < 32)
			{
				xing->flags = 0;
				return -1;
			}

			xing->bytes = mad_bit_read(&ptr, 32);
			bitlen -= 32;
		}

		if (xing->flags & XING_TOC) {
			int i;

			if (bitlen < 800)
			{
				xing->flags = 0;
				return -1;
			}

			for (i = 0; i < 100; ++i)
				xing->toc[i] = (unsigned char) mad_bit_read(&ptr, 8);

			bitlen -= 800;
		}

		if (xing->flags & XING_SCALE) {
			if (bitlen < 32)
			{
				xing->flags = 0;
				return -1;
			}

			xing->scale = mad_bit_read(&ptr, 32);
			bitlen -= 32;
		}

		return 0;
	}

	int scanHeader(struct mad_header *header, struct xing *xing)
	{
		struct mad_stream stream;
		struct mad_frame frame;
		unsigned char buffer[8192];
		unsigned int buflen = 0;
		int count = 0, result = 0;

		mad_stream_init(&stream);
		mad_frame_init(&frame);

		if (xing)
			xing->flags = 0;

		while(1)
		{
			if(buflen < sizeof(buffer))
			{
				int64 bytes = input->read(buffer + buflen, sizeof(buffer) - buflen);

				if(!bytes)
				{
					result = -1;
					break;
				}

				buflen += int(bytes);
			}

			mad_stream_buffer(&stream, buffer, buflen);

			while(1)
			{
				if(mad_frame_decode(&frame, &stream) == -1)
				{
					if(!MAD_RECOVERABLE(stream.error))
						break;

					continue;
				}

				if(count++ || (xing && parseXing(xing, stream.anc_ptr, stream.anc_bitlen) == 0))
					break;
			}

			if (count || stream.error != MAD_ERROR_BUFLEN)
				break;

			memmove(buffer, stream.next_frame, (buflen = &buffer[buflen] - stream.next_frame));
		}

		if(count)
		{
			if(header)
				*header = frame.header;
		}
		else
			result = -1;

		mad_frame_finish(&frame);
		mad_stream_finish(&stream);

		return result;
	}

private:

	struct xing _xing;				/* Xing VBR tag data */

	struct mad_stream _madStream;		/* MAD stream structure */
	struct mad_frame _madFrame;			/* MAD frame structure */
	struct mad_synth _madSynth;			/* MAD synth structure */

	int _samplesLeftInBuffer;
	int64 _streamSize;
	int64 _lastSample;
	int64 _streamStartPosition;
	mad_fixed_t *_leftChannel;
	mad_fixed_t *_rightChannel;

	mad_timer_t _madCurTime;				/* current playing time position */
	mad_timer_t _madTimeLength;		/* total playing time of current stream */

	unsigned char _buffer[40000];	/* input stream buffer */
};

static const tchar* const extensions[] =    { T(".mp3"), T(".mp2"), T(".mpg"), 0 };
MADAudioFormat::MADAudioFormat() : AudioFormat(T("MPEG Audio Decoder (MAD)"), (const tchar**) extensions)
{
}

MADAudioFormat::~MADAudioFormat()
{
}

bool MADAudioFormat::canHandleFile(const File& fileToTest)
{
	return fileToTest.hasFileExtension(T("mp3")) ||
			fileToTest.hasFileExtension(T("mp2")) ||
			fileToTest.hasFileExtension(T("mpg"));
}

const Array<int> MADAudioFormat::getPossibleSampleRates()
{
    const int rates[] = { 44100, 48000, 0 };
    return Array <int>(rates);
}

const Array<int> MADAudioFormat::getPossibleBitDepths()
{
    const int depths[] = { 16, 24, 32, 0 };
    return Array<int> (depths);
}

bool MADAudioFormat::canDoStereo()
{
	return true;
}

bool MADAudioFormat::canDoMono()
{
	return true;
}

bool MADAudioFormat::isCompressed()
{
	return true;
}

AudioFormatReader* MADAudioFormat::createReaderFor(InputStream* sourceStream, const bool deleteStreamIfOpeningFails)
{
	AudioFormatReader* r = new MADAudioFormatReader(sourceStream);

    if (r->sampleRate == 0)
        if (!deleteStreamIfOpeningFails)
			delete r;

    return r;
}

AudioFormatWriter* MADAudioFormat::createWriterFor(OutputStream* streamToWriteTo, double sampleRateToUse, unsigned int numberOfChannels, int bitsPerSample, const StringPairArray& metadataValues, int qualityOptionIndex)
{
	return 0;
}
