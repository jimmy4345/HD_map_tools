# las2pcd_all_in_one

Author : **Yen En Huang**, POINT Lab, Geomatics NCKU, Taiwan

## Overview
Simple function written in C++ used to convert multiple .LAS point cloud data to PCL-compatible .PCD format.

## Required dependencies
* PCL (http://pointclouds.org/)
* ~~libLAS (https://www.liblas.org/)~~ (**Ubuntu 18.04 and older**)
* LAStools (http://lastools.org) (**Ubuntu 20.04 and upper**)

## How to install dependencies

### Ubuntu 18.04 installation and build: (No longer active)

```
sudo apt-get update
sudo apt-get install -y git cmake
sudo apt-get install -y libpcl-dev liblas-dev liblas-c-dev
git clone https://github.com/jimmy4345/las2pcd.git
cd las2pcd
mkdir build && cd build
cmake ..
make -j8
```

### Ubuntu 22.04 installation: (**active**)

#### Install pcl dependencies 

```
sudo apt-get update
sudo apt-get install -y git cmake
sudo apt-get install -y libpcl-dev
```

#### Install LAStools dependencies

```
git clone https://github.com/LAStools/LAStools.git
cd LAStools
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j8
sudo make install
```

#### Build

```
git clone http://140.116.80.14:8080/jimmy/point_cloud_tools.git
cd point_cloud_tools
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j8
```

## Parameters

### Core Parameters

| Name                        | Type                   | Description                                                          |
| --------------------------- | ---------------------- | -------------------------------------------------------------------- |
| `input_dir`                 | string                 | The directory where you put LAS file and want to convert             |
| `output_file_name`          | string                 | The output pcd file name                                             |
| `offsetX`                   | double                 | Offset that you want to move in X axis (TWD97)                       |
| `offsetY`                   | double                 | Offset that you want to move in Y axis (TWD97)                       |
| `offsetZ`                   | double                 | Offset that you want to move in Z axis (TWD97)                       |
| `leaf_size`                 | double                 | The parameter for downsample using Voxel Grid Filter                 |
| `point_type` (developing)   | int                    | The pcd format you want to convert (1:PointXYZI, 2:PointXYZRGB)      |


## How to use:

Edit your config file in config directory

```
cd build
./las2pcd 
```
