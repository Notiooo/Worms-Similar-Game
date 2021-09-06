## Worms Similar Game
The game was created for the project during the 4th semester of studies in computer science at the Silesian University of Technology. The time I spent on the project oscillated around 4 - 5 weeks.
<div align="center">
<a href="https://www.youtube.com/watch?v=iI0FcAPFU6o"><img src="docs/instructions/demo.gif" alt="Gif presenting the game"/></a>
<p>A <a href="https://www.youtube.com/watch?v=iI0FcAPFU6o">video</a> showing how the game works</p>
</div>

### Source of Knowledge
My main knowledge comes from the documentation of individual libraries. In the process of learning (started some time ago) I also read two books: *C++ Primer 5th edition* and *SFML Game Development.* I also get a lot of help from the official SFML forum. There are many helpful threads there.

### Used Libraries
* SFML Library -- under the [zlib/png license](http://opensource.org/licenses/Zlib "Terms of the zlib/png license")
* Box2D Library -- under the [MIT license](https://opensource.org/licenses/MIT "Terms of the MIT license")
* Clipper Library -- under the [BSL-1.0 license](https://opensource.org/licenses/BSL-1.0 "Terms of the BSL-1.0 license")
* Poly2Tri Library -- under the [BSD-3-Clause license](https://opensource.org/licenses/BSD-3-Clause "Terms of the BSD-3-Clause")

And [Doxygen](https://www.doxygen.nl/index.html) as standard tool for generating documentation -- under the [GNU General Public License](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)

### How to install
#### Windows
On Windows, simply run the [buildVSProject.bat](buildVSProject.bat) file. This will create the Visual Studio project files. You can then open the project in Visual Studio and compile.

#### Linux
On Linux you will most likely need to give the appropriate permissions to [buildMakeFiles.sh](buildMakeFiles.sh) and [Worms-Clone/vendor/bin/premake/premake5](Worms-Clone/vendor/bin/premake/premake5).

Then we perform:
> \$ ./buildMakeFiles.sh   
> \$ make

### How to play
[Instructions on how to play the game](docs/instructions/How_to_play.pdf)

### The report
[A report describing the status of the project as at day of 18.04.2021](docs/instructions/Worms_Clone_Computer_Programming_Final_Project.pdf)
