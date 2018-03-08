# Talk to Light Utilities

To run Talk to Light as an installation, we added 2 utility applications.

## Monitor

Project folder: "TTL_Monitor"

While running a live event, it's always good to know if your application is still running. This application receives a heartbeat via osc from the Raspberry Pi app. If no heartbeat is received, we know something is up.

## Instructions & Music

Project folder: "TTL_musicControl"

This application has two functions.
- It shows instruction on two screens in the room, making some suggestions on what to ask the Google Assistant and how to trigger different light modes.
- There's also music playing in the room. With this application we can control the volume based on the interaction with the Assistant as well as based on the current light mode. You definitely need some music when you are talking to a disco ball, right!?
