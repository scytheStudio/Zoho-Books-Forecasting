# Zoho Forecasting

The application is used for displaying a prognosis of the company budget. The need for this application is due to inability of Zoho Books frontend to generate budget forecasting report.

## Documentation

### Table of contents

- [Files](#Files)
- [Installing](#Installing)
- [Setup](#Setup)
- [Usage](#Usage)

### Files

- **Doxyfile** - configuration file for Doxygen.

- **doxygen-awesome.css** - style sheet file describing the default appearance of Scythe Studio documentation.

- **doxygen-awesome-sidebar-only.css** - style sheet changing navigation to more modern sidebar.

- **Scythe_Studio_logo.png** - Scythe Studio logo added to the documentation.

### Installing

1. Download Doxygen from [Official Doxygen Site](https://www.doxygen.nl/download.html)

2. Add Doxygen to PATH in your system environment variables.

3. Install Doxyqml through pip3 (need to install Python first)

   `pip3 install doxyqml`

4. Add Doxyqml to PATH in your system environment variables.

5. Install graphviz from [Official Graphviz Site](https://www.graphviz.org/download/) (version > 1.8.10)

6. Add graphviz to PATH in your system environment variables.

## Setup

The first thing is to place the files in the root folder of your current project. You can create a folder for this, e.g. _"documentation-config"_

Next you need to modify the **Doxyfile** file. You can do this through build-in Doxygen program called **Doxywizard** (if you are using this method, select expert mode) or use any text-editor (i.e. VSCode).

## Important fields to modified in _Doxyfile_

- _PROJECT_NAME_ -> Set a current project name.

- _PROJECT_NUMBER_ -> Set an actual project version.

- _PROJECT_BRIEF_ -> Add a short description to your project.

- _INPUT_ -> Sets the folders where the code for the documentation is located. So in 99% cases you should add **src** folder and **qml** folder. **IMPORTANT** -> Remember that if the folder you specify contains _build_ folder, doxygen will also scan it, which will generate an incorrect documentation. In that case, add this directory to _EXCLUDE_)

### Optional fields to modified in _Doxyfile_

- _PROJECT_LOGO_ -> If the current project has a logo, you can set it up. By default the Scythe Studio logo will be used.

- _OUTPUT_DIRECTORY_ -> Set the destination folder for the documentation. By default a new folder "documentation-generated" will be created.

Once these variables are set, your configuration file is ready. Now you can proceed with creating the documentation.

## Usage

To generate the documentation you can use **Doxywizard** (open **Doxyfile** and hit run) or command line.

`doxygen Doxyfile`

It may take a while because of generating graphs and diagrams. Documentation should be generated inside the output directory. To view it, find _index.html_ file and open in the browser.
