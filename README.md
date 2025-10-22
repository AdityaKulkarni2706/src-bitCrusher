# src-bitCrusher

The APVTS is a parameter management system for JUCE framework. 

__You effectively need to understand 3 things in order to seamlessly integrate Frontend and Backend of your audio plugins.__ 

1) The  `APVTS` object
2) The `createParameterLayout` function
3) The `Attachment` object

The APVTS Object:
1) Takes in a reference of your audio processor class. Specifically, `*this` gives you the reference to your current class which you can send as a parameter for the APVTS Object.
2) Undo Functionality : Most likely you wouldn't need to use this parameter. It deals with the undo functionality (obviously), and is usually set to  `nullptr` 
3) A name for your parameters, I usually just write "PARAMETERS". Just a name for your APVTS object really. 
4) The most important argument is the `createParameterLayout()` function. It is a function you define in your Audio Processor class, which returns a parameter layout of your plugin (yes literally). Essentially it is a part of the APVTS class itself, rather a sub-class of it (I am not entirely sure, it uses the `::` syntax)

The APVTS object is initialized in the constructor of your processor class. 

Before we go ahead with APVTS, understanding `createParameterLayout()` is really necessary.
Given below is the example of one such function,

Gist of whatever I yapped below,

`createParameterLayout` : You literally create a `vector` (not an actual vector but analogies help) of `AudioParameter` objects, and return that. That's it!


`juce::AudioProcessorValueTreeState::ParameterLayout  |return-type|               BitCrusherAudioProcessor::createParameterLayout()    |fn-scope and fn-name|                              

	{
		
    juce::AudioProcessorValueTreeState::ParameterLayout layout{}; |inst. of layout|

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{ "Probability", 1 },
            "Probability",
            juce::NormalisableRange<float>(0.0f, 0.75f, 0.01f),
            0.3
        )
    );

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{ "Density", 1 },
            "Density",
            juce::NormalisableRange<float>(0.0f, 0.75f, 0.01f),
            0.3
        )
    );

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{ "Window", 1 },
            "Window",
            juce::NormalisableRange<float>(0.0f, 64.0f, 1.0f),
            16.0f
        )
    );
    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{ "Wet", 1 },
            "Wet",
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
            0.5
        )
    );

    return layout; 
	}

Layout object essentially holds a set of unique pointers which point towards Audio Parameter Objects, so you can really just think of it as a sequence of Audio Parameter Objects in a sequence like a vector or something, but probably dynamically allocated. The Audio Parameter object itself contains description about the audio parameter such as its ID, version (not important right now, only for version control and rollbacks), its name, range, default value. 
Note : All of the values are just numbers which can be controlled, they are not any magic effect of any sort. Window is a parameter in the example which is solely just a number for layout or even for the layout itself. What makes it 'window' is not a 'What' question, more like a 'Where' question. It becomes a window during the core logic of the program, which is usually either a function of the Audio Processor class or literally in the process block.

A simpler example would be a gain knob. The gain parameter is absolutely only a number to the layout! The place where it is 'gain' is when we are multiplying it with samples. 

FINALLY,
The `SliderAttachment` class.

So basically, how are you gonna send your knob readings to your processor class? 
This is how.

Where do your parameter objects live? The APVTS object.
All you have to do is,
1) Get `APVTS` object (Usually its just `ur AudioProcessorClass.apvts`. If you remember, we initialized the `APVTS`class in the constructor of the audio processor class. Its the same `apvts` instance, obviously, it has to be )
2) Get the parameter name you wanna link to your frontend, could be just "GAIN".
3) Finally, you need to get the object with which you can change/vary your gain. Could be a slider? Yes.

Remember the following which I said above. We will get back to it!

Now, what you *`reaaaally`* need is something which can connect your APVTS object, and your Parameter. This *something* is called an `Attachment`

Now, I do not know how an attachment works, so imma black box it **for now**. 
All I know is, you need a unique pointer to an attachment firstly.
As far as I know, Attachments could be of different types. The mostly commonly used attachment is a `SliderAttachment`which is used if you have a knob for varying the parameter.
You declare all of these things in the header file (unwritten rule).
One example could be,
`std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetAttachment;`

The way you **connect** your APVTS to your Knob is by sending *those* things in as parameters to the `SliderAttachment` object for that specific slider. a slider attachment for gain or something maybe.

Example Syntax:

`wetAttachment = std::make_unique<SliderAttachment>(
    `audioProcessor.apvts, "Wet", wetSlider`
`);`

I will be honest, I am not yet certain to why `make_unique` is used. But I know it is right. 

Essentially, 
You give your attachment the parameter name, the `apvts` instance and your parameter varying object (slider object in this case), and it DOES THE LINKING FOR YOU.

That is mostly about it! 
