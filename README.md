Our mannequin extra credit includes a creative scene, requiring the mesh Haku_Yowane.pmd, and running the 
Creative_scene.json file. It shows Haku Yowane trying to walk but falling like a drunk. Best viewed from the front.

Additionally, if you click ctrl-a, you can see a gui dialog box pop up and type in the file.json you would like 
to save your animation as.

We also implemented export as movie; however, the frames slide up, something we likely think is a GPU issue since we
asked other teams about the implementation, and have implemented very similarly. We saw a similar gpu dependent issue 
while I ran on my AMD chip on OSX regarding multi-sampling, which is why we think it's gpu dependent. In order to create
an MP4 movie, click ctrl-m, allow the animation to play through, and open output.mp4. This requires the ffmpeg library to work on 
the build computer and it must be a Mac. Uncomment the chunk of code on line 629 to run it, and uncomment lines 741, 742, and 747.

We also implemented anti-aliasing for the main view and anti-aliasing for the preview. Once implemented, the edges of the 
mesh figure become more smooth.

Loading from json causes previews to be incorrectly shown, but the animation is loaded properly, and previews can be 
updated manually via space bar as needed. 
