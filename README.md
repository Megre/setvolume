
 `setvolume` is a simple tool to get and set audio status (volume and mute) on Windows (Windows Vista / Windows Server 2008 or later, desktop apps only).

# Usage
----------
Command parameters:

setvolume [get | set] [volume] [mute]

	get: return current volume status
	volume: set volume value ([0, 100])
	mute: set mute status (0 to maintain, -1 to mute, 1 to speak)
	
Usage examples:
    
    setvolume get
    
returns "20, mute", which means current volume is 20 in mute state.

    setvolume set 30 1
    
sets the volume to 30, non-mute state.