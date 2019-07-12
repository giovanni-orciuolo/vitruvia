# README #

Vitruvia is cheat with multiple features for CS:GO.
This project is abandonware, and now this repository is its grave. Rest in peace.

This repository contains the entire source code of the cheat.
The source code is divided in 2 large parts:

SOURCE SDK:

To declare all the interfaces, classes, functions, everything that the game uses internally and that we can use.
Interfaces are properly divided based on the module they are located in.
Adding an interface, however, requires to edit both the Source SDK and the Vitruvia SDK.

Every single file in the Source SDK part is part of the Source namespace. You can add things to the namespace by simply using
the synatx


```
#!C++

SOURCE_INIT

// Code here

SOURCE_END
```


See the files for examples.
You can then access the elements of the SDK by writing Source::

VITRUVIA SDK:

This part of the cheat declares and implements everything that is related to cheats: features, interface implementation, classes, etc. It includes everything from
the Source SDK and it uses it.
Same as the Source SDK, the Vitruvia SDK is defined by


```
#!C++

namespace Vitruvia {

// Code here

}
```


You can access the namespace by writing Vitruvia::

### How can I implement a new interface? ###

First of all find out from what DLL does this interface come. Inside my personal Source SDK source, I divided all the interfaces so they match the DLL they come from.
Inside SourceSDK/Interfaces you will find several folders named like "Client", "Engine", etc... You get the point.
Remember to always place the source of the interface inside the right folder. To avoid name clashes and some other bad stuff, watch other interface sources to get an idea on how to format your new one.
Always remember to add the "SourceSDK" ascii art on top of every Source SDK related file you add! You can copypaste it easily from other files.

Once you added this new interface, go to "SourceSDK.h" and add it to the includes. Now to grab it go to Vitruvia/InterfaceList/InterfaceList.h;
Now add a new line

```
#!C++

// Other IFACE_EX...
IFACE_EX(your_interface_class, your_interface_name)

```

Now head to InterfaceList.cpp and add a new IFACE line, doing the exact same thing you did in the .h file
Inside SetupInterfaces, you can grab your interface by different means:

```
#!C++

IFACE_GET(iface, module, iface_name, type)
//- It will call GetInterface to find the interface you are looking for. You don't need to provide the interface version, it's all automated!
//(The GetInterface function is using a blazing fast InterfaceReg based method to find interfaces immediately. Automated doesn't mean "bruteforcing")

FindPattern(ida_pattern)
//- Call FindPattern to grab the offset for the interface.
//Remember that offsets should always be stored inside Vitruvia/Offsets so yeah plz do it.

```

### Who do I talk to? ###

If you have problems or questions, just contact the repo owner double v.
If you are here, it means that you already know him and that you already have a contact with him.
