##Poco in this project
This project uses the C++ -library Poco, available at pocoproject.org and licensed with the Boost Software License.  
This directory holds the modified versions of the cmake-files distributed with the libraries, other files are unchanged since the download.
Mainly the changes to cmake-files are to control the output directory (to put the compiled libraries somewhere not at risk of being written over) and to make the libraries static.