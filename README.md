# imgui-logger

This project includes a logger for IMGUI that has the following features:  
There are 2 log types: General and Game  
There are 3 log levels: Info, Warning, Error  
Logs can be filtered by level, cleared, saved to file, or copied to clipboard  
The user can also type in the console input box to log custom messages  

## Aproach

I wrote the logger class in 2 seperate files because that is what I'm used to in C++. I hard coded the methods for logging all levels and sources because it felt like the most readable and simple approach for the requirements given. I had to ask the instructor for clarification reguarding the options button and its inteded functionality. I referenced example code from the IMGUI demo to create the IMGUI windows.  

### Usage

This was built and tested using macOS and built using cmake. All output files will be created in the build folder.
