SOM algorithm binary for VO-CLOUD worker

## Installation and Usage

1. Compile the application by running a compile script: `./compile.sh`
1. Insert data files into input folder Wrapper/input/.
1. Arbitrary change the user configuration file config.json.
1. Run the application: `$> python run.py`
1. Look for a results in result file Wrapper/result.
1. Arbitrary run created web application by opening Wrapper/result/index.php file in your browser.


## Parameters of configuration file

Name arbitrary name of the experiment.
* **algorithm** definition of algorithm
  * **BMU** "normal" or "probing" algorithm
  * **threads** select number of computational threads (integer)
* **data** definition of data input
  * **path** one or more paths to input files or whole directories.
  * **file_type** "csv" or "fits"
  * **delimiter** delimiter of csv file.
  * **columns** columns in file or dimension of input vector.
* **parameters** SOM parameters
  * **topology** "rect" for rectangular, "hex" for hexagonal
  * **neighborhood_fcion** "gaussian" or "bubble" function
  * **neighborhood_radius** size of initial neighborhood radius (integer)
  * **map_size_x** horizontal size of SOM grid (integer)
  * **map_size_y** vertical size of SOM grid (integer)
  * **iteration** number of iterations over whole data set (integer)
  * **learning_rate** initial learning rate (float in interval 0–1)
  * **probing_iter** number of iterations of probing algorithm (integer, used only if probing algorithm is selected)
* **output** selection of output files
  * **visualization** values true or false, create visualization or not.
  * **error** values true or false, count errors or not.
  * **web** values true or false, create web application or not.
* **optional** info selection of optional functionality
  * **Names** values true or false, add files with data names or not (number of files and rows must be equivalent to input data)
  * **Names_path** one or more paths to names files or whole directories
  * **Classes** values true or false, add files with classes or not (number of files and rows must be equivalent to input data)
  * **Classes_path** one or more paths to classes files or whole directories

## Input Files

Input data has to be in CSV format. In case of having solely FITS files, an included converter can
be used. The data sets can be separated in arbitrary number of files. They common size has no
limitations as the files are read sequentially. However, the average time of running is slightly
bigger when using more files, as a data have to be for each iteration repeatedly read from the disk.
Optionally, it can be added the file containing data classes and names. It is option that can help
recognize quality of chosen algorithm (by visualizating it). This data files should be equivalent to
main data in a way how they are separated in files and rows. It is important to add, that class and
name data are not used in training phase of algorithm. They are exclusively used for visualization.
