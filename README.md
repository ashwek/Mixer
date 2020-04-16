# Mixer

# Stereo to Mono

[stereo_to_mono.cpp](https://github.com/ashwek/Mixer/blob/master/stereo_to_mono.cpp) is a C++ code which can convert a **stereo Wav file** to a **mono Wav file**.

[Difference between mono & stereo - stackexchange.com](https://music.stackexchange.com/questions/24631/what-is-the-difference-between-mono-and-stereo/24635)

![Wav file format](http://soundfile.sapp.org/doc/WaveFormat/xwav-sound-format.gif.pagespeed.ic.tIS-Bqb8Y1.png)

Data is stored in channels in a wav file. If a wav file has 2 channels, both the channels may have different content & it may sound different while using headphone as difference can be spotted through the left & right side of the headphone. On a single speaker device, both the channels are combined and played.

[Virtual Barber Shop Haircut - soundcloud.com](https://soundcloud.com/tmayne/virtual-haircut) best example of how having 2 different channels can be beneficial and produce some great content.

But some people prefer mono over stereo. Stereo can be converted to mono by mixing both the channels of the stereo and forming a single mono channel. The data read from wav file has to be converted into Fractional Binary Fixed-Point Numbers[\[3\]](#3) before mixing it into a single channel.

<table>
  <tr>
    <th>Input Stereo file</th>
    <td><a href="https://github.com/ashwek/Mixer/blob/master/demo/stereo_file.wav">demo / stereo_file.wav</a></td>
  </tr>
  <tr>
    <th>Wave of input<br />stereo file</th>
    <td><img src="https://raw.githubusercontent.com/ashwek/Mixer/master/demo/stereo_wave.png" height="200" /></td>
  </tr>
  <tr>
    <th>Output Mono file</th>
    <td><a href="https://github.com/ashwek/Mixer/blob/master/demo/mono_file.wav">demo / mono_file.wav</a></td>
  </tr>
  <tr>
    <th>Wave of ouput<br />mono file</th>
    <td><img src="https://raw.githubusercontent.com/ashwek/Mixer/master/demo/mono_wave.png" height="100" /></td>
  </tr>
</table>

----

### References

<a name="1" />[1. Microsoft WAVE soundfile format - soundfile.sapp.org](http://soundfile.sapp.org/doc/WaveFormat/)<br />
<a name="2" />[2. Wav (RIFF) File Format Tutorial - topherlee.com](http://www.topherlee.com/software/pcm-tut-wavformat.html)<br />
<a name="3" />[3. Number Systems for Digital Audio - dsprelated.com](https://www.dsprelated.com/freebooks/mdft/Number_Systems_Digital_Audio.html)<br />
