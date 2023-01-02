# Example of how to access the music libary on iOS devices

Demonstrates how to load and play files from the user's music library. The files can be played as a stream or converted to wav before being played. 

Note that this will only work on physical devices, not on the simulator.

The Info.plist must include an 'NSAppleMusicUsageDescription'. This can be done by adding the following to the Projucer's 'Custom Plist' field: 


```
<plist> 
<dict>
<key>NSAppleMusicUsageDescription</key>
<string>This app requires access to the music library.</string>
</dict>
</plist>
```

