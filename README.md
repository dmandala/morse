Morse
=====

## The Story

    I found a client that had a specific need that could not be met at the
    local electronics store or Amazon.  He wanted a portable device that
    could transmit paragraphs of text as Morse code at different speeds and
    loop forever.  He need both the hardware and the software to be solved.

    So I designed the hardware first, I got a waterproof case, added a 1080P
    5VDC monitor, Raspberry Pi, a semiconducter relay, a 100W LED spot
    light, a 12V 30A power supply and buck power supply that could convert
    12VDC - 24VDC into 5VDC for the Raspberry Pi, did some fabrication with
    aluminum flat and angle, rivets and screws and we had a portable system
    that had the capability of sending Morse Code via light.  

    Now I needed software to run the hardware, thus "morse" was born.

    "morse" is a C command line application that can read an ascii text file
    understand what can be sent at Morse Code and what can't, the program
    does something a bit unusual.  Morse Code is only uppercase letters, but
    uppercase letters are really hard to read when you are typing a
    paragraph of text with nothing but all uppercase text.  To make it
    easier to create and read text with the system the program treats upper
    and lower case letters exactly the same as if they were uppercase only.

    The other thing it does is handle spaces and carriage returns (\n new
    lines) as if they are timing between words.  So you don’t need to just
    type one long line but can break up the text you want to send into
    easily readable lines and it will be sent correctly.

    Finally the program will ignore any text that is not in the Morse code
    table.  So if you enter any text that is not capable of being send as
    Morse code it will be silently ignored.

## Quick Primer on Morse Code

    Morse Code is all about the dot.  All the timing in Morse code is based
    off the time of the dot.  So:

         A dot is one time unit.
         A dash is three dot time units.
         A dot dot or dot dash or dash dash seperation is one dot time unit.
         A letter seperation is three dot time units.
         A word seperation is seven dot time units.

    How fast a dot is depends on how many words per minute you are sending,
    and from there cascades the rest of the timing.  There are two type of
    timing: "Standard" and "Farnsworth", the "morse" application can do both
    types.  For more info lookup Morse Code on a search engine.

## Features

    Runs on a Raspberry Pi and flashes Morse Code via an an LED light
    attached to a GPIO pin.  The LED is 100W's and requires between 
    12VDC - 30VDC to power up the spot light.

    "morse" will take options from /etc/morsecode.cfg and from
    .morsecode.cfg in the users home directory.  The dot file overides the
    /etc file. So it's entirely possible to specify everything in a config
    file and just call morse on the command line and it will run the text
    file specified.

    There is no reason that this HAS to run on a Raspberry Pi, it could run
    on any computer with GPIO pins, though you would need to replace the
    piGPIO library with something that runs on your hardware.

## Usage

    Morse may be called with command line options.
          -s Sets the speed of Morse Code in word per minute WPM, from 1 - 75
          -t Sets Standard Morse coding or Farnsworth Morse coding \"S or F\"
          -l ("lower case L") Sets the loop count, 0 loops forever anything else up to 1000 loops that many times
          -p Sets the port used to activate the LED light. It's preset to 8
          -f Sets the text file to be set as Morse code. File paths are allowed
          -h or -H displays this text.
      morse -s5 -tS -l1 -fexample.txt
    Sets the:\n    Speed at 5WPM\n    Standard Morse code timing\n    1 loop
        Uses the \"example.txt\" to be set via the LED light as Morse code flashes

    There is a .morsecode.cfg file in the users directory that can set these
    options as standard so you don't have to set them each time.  You can
    use the text editor to edit the file.

## The source code

    All C, 5 actual C files, and 2 local header files.  The code currenly defaults to GPIO port 8
    which happened to be the port I used to hook up the LED light.


    morse.h		    Main header file with the primary struct defined
    morse_code.h	    Morse Code table defined (don't mess with this)
    display_morse.c	    File that does the timing and display of Morse code
    morse.c                 Start up file
    process_command_line.c  Does what it says
    process_config_file.c   Does what it says
    process_file.c	    Maps the text file into memory.

    Makefile		    Handy make file
    README.md		    This file
 
## Requires

    Morse requires the piGPIO library and the libconfuse library to compile
    and run.  Both of those library's are compatible with the GPLv2 license.

    Information about the pigpio library can be found at:
    http://abyz.me.uk/rpi/pigpio/ and the source code can be cloned from:
    git clone https://github.com/joan2937/pigpio
    
    Morse requires the use of the daemon pigpiod run as root.

    libConfuse is a configuration file parser library, information about
    libconfuse is at: https://github.com/martinh/libconfuse and the source
    code can be cloned from: https://github.com/martinh/libconfuse.git

## License

    The GPLv2 license only, no later versions. See GPLv2-LICENSE.txt
