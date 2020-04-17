# Mixer

**Ever thought of listening to 2 songs at the same time??** Well now you can.

Wav file can have multiple channels. If a wav file has 2 channels, & you are using headphones to listen to the audio, both the channels are played independently on the left & right side of the headphones.

[mix.cpp](https://github.com/ashwek/Mixer/blob/master/mix.cpp) takes 2 wav files as input and produces a third wav file as output. This output wav file has 2 channels. The first channel is composed of the the first input wav file & the 2nd channel is composed of the 2nd input wav file.

So when you listen to the output wav files, you can listen to 2 different songs (audio files) at the same time. One playing on the left side of the headphone & the other on the right side of the headphone.

<table>
  <tr>
    <th colspan="2">Demo</th>
  </tr>
  <tr>
    <th>Input File 1</th>
    <td><a href="https://github.com/ashwek/Mixer/blob/master/demo/song1.wav">demo / song1.wav</a></td>
  </tr>
  <tr>
    <th>Input File 1<br />Wave diagram</th>
    <td><img src="https://raw.githubusercontent.com/ashwek/Mixer/master/demo/song1%20-%20wave.png" height="200" /></td>
  </tr>
  <tr>
    <th>Input File 2</th>
    <td><a href="https://github.com/ashwek/Mixer/blob/master/demo/song2.wav">demo / song2.wav</a></td>
  </tr>
  <tr>
    <th>Input File 2<br />Wave diagram</th>
    <td><img src="https://raw.githubusercontent.com/ashwek/Mixer/master/demo/song2%20-%20wave.png" height="200" /></td>
  </tr>
  <tr>
    <th>Output File</th>
    <td><a href="https://github.com/ashwek/Mixer/blob/master/demo/song1%20%2B%20song2.wav">demo / song1 + song2.wav</a></td>
  </tr>
  <tr>
    <th>Output File<br />Wave diagram</th>
    <td><img src="https://raw.githubusercontent.com/ashwek/Mixer/master/demo/song1%20%2B%20song2%20-%20wave.png" height="200" /></td>
  </tr>
</table>
<br />

----
<br />  

# Stereo to Mono

[stereo_to_mono.cpp](https://github.com/ashwek/Mixer/blob/master/stereo_to_mono.cpp) is a C++ code which can convert a **stereo Wav file** to a **mono Wav file**.

[Difference between mono & stereo - stackexchange.com](https://music.stackexchange.com/questions/24631/what-is-the-difference-between-mono-and-stereo/24635)

![Wav file format](http://soundfile.sapp.org/doc/WaveFormat/xwav-sound-format.gif.pagespeed.ic.tIS-Bqb8Y1.png)

Data is stored in channels in a wav file. If a wav file has 2 channels, both the channels may have different content & it may sound different while using headphone as difference can be spotted through the left & right side of the headphone. On a single speaker device, both the channels are combined and played.

[Virtual Barber Shop Haircut - soundcloud.com](https://soundcloud.com/tmayne/virtual-haircut) best example of how having 2 different channels can be beneficial and produce some great content.

But some people prefer mono over stereo. Stereo can be converted to mono by mixing both the channels of the stereo and forming a single mono channel. The data read from wav file has to be converted into Fractional Binary Fixed-Point Numbers[\[3\]](#3) before mixing it into a single channel.

<table>
  <tr>
    <th colspan="2">Demo</th>
  </tr>
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
