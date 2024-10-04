//C++
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <cstring>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <fstream>
#include <sstream>
#include <map>

//LAStools
#include <lasreader.hpp>
#include <laswriter.hpp>

//PCL
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/common/common.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/octree/octree_search.h>

using namespace std;
using namespace pcl;

char *getExtension(char *fileName) 
{
	int len = strlen(fileName);
	int i = len;
	while (fileName[i] != '.' && i > 0) { i--; }

	if (fileName[i] == '.') {
		return &fileName[i + 1];
	}
	else {
		return &fileName[len];
	}
}

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) 
	{
		//cout << string(dirp->d_name) << '\n';
		if (strcmp(getExtension(dirp->d_name),"las") == 0)
		{
			files.push_back(string(dirp->d_name));
		}
    }
    closedir(dp);
    return 0;
}

map<string, string> readConfig(const string& filename) {
    map<string, string> config_parameters;
    ifstream config_file(filename.c_str());

    if (!config_file.is_open()) {
        cerr << "Failed to open config file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(config_file, line)) {
        istringstream line_stream(line);
        string key, value;
        getline(line_stream, key, '=');
        getline(line_stream, value);
        config_parameters[key] = value;
    }

    config_file.close();
    return config_parameters;
}

PointCloud<PointXYZI>::Ptr subFilter(pcl::PointCloud<PointXYZI>::Ptr pointcloud, pcl::IndicesPtr indices, double cell_x, double cell_y, double cell_z) {
    pcl::PointCloud<PointXYZI>::Ptr filtered_pointcloud(new pcl::PointCloud<PointXYZI>);
    pcl::VoxelGrid<PointXYZI> sor;
    sor.setInputCloud(pointcloud);
    sor.setIndices(indices);
    sor.setLeafSize(cell_x, cell_y, cell_z);
    sor.filter(*filtered_pointcloud); // No problem :)
    return filtered_pointcloud;
}

PointCloud<PointXYZI>::Ptr OctFilter(pcl::PointCloud<PointXYZI>::Ptr cloudIn, double cell_x, double cell_y, double cell_z) {    
    pcl::octree::OctreePointCloudSearch<PointXYZI> octree(32);  // // Octree resolution - side length of octree voxels
    octree.setInputCloud(cloudIn);
    octree.addPointsFromInputCloud();
    pcl::PointCloud<PointXYZI>::Ptr filtered_cloud(new pcl::PointCloud<PointXYZI>);    
    for (auto it = octree.leaf_depth_begin(); it != octree.leaf_depth_end(); ++it)
    {

        pcl::IndicesPtr indexVector(new std::vector<int>);
        pcl::octree::OctreeContainerPointIndices& container = it.getLeafContainer();

        container.getPointIndices(*indexVector);
        *filtered_cloud += *subFilter(cloudIn, indexVector, cell_x,cell_y,cell_z);
    }
    return filtered_cloud;
}


int main (int argc, char *argv[])
{
    cout << "====================================================================" << endl;
	cout << " LAS2PCD - Converts .las point clouds into PCL-friendly format .pcd " << endl;
    cout << " Workflow : las -> movement (pcd) -> downsample -> merge -> pcd " << endl;
	cout << " **Ubuntu 18.04 are no longer use (Maybe new way can use). " << endl;
    cout << " liblas-dev -> LAStools. Work in Ubuntu 20.04 & 22.04 " << endl;
	cout << "====================================================================" << endl;
	cout << endl;

    map<string, string> config = readConfig("../config/config.txt");

    string input_dir = config["input_dir"];
    string output_file_name = config["output_file_name"];
    double offsetX = stod(config["offsetX"]);
    double offsetY = stod(config["offsetY"]);
    double offsetZ = stod(config["offsetZ"]);
    double leaf_size = stod(config["leaf_size"]);
    string leafsize = config["leaf_size"];
    // int point_type = stoi(config["point_type"]);

    vector<string> files = vector<string>();
    getdir(input_dir,files);


    PCDWriter writer;
    PointCloud<PointXYZI>::Ptr merged_cloud(new PointCloud<PointXYZI>);

    for(int i = 0; i < files.size(); i++)
    {
        string lasfile_path = input_dir + "/" + files[i];
        //cerr << "INFO : Loading : " << lasfile_path << endl;       

		LASreadOpener lasreaderopener;
		lasreaderopener.set_file_name(lasfile_path.c_str());
		unique_ptr<LASreader> lasReader(lasreaderopener.open());

        if (!lasReader) {
            std::cerr << "ERROR : Impossible to open the file : " << lasfile_path << std::endl;
            return 1;
        }

        unsigned long int pointAmount = lasReader->header.number_of_point_records;
        cerr << " PointCloud detect: " << pointAmount << " data points in " << lasfile_path << endl;

        cerr << " Start movement..." << endl;
		PointCloud<PointXYZI>::Ptr cloud(new PointCloud<PointXYZI>);
        cloud->points.reserve(pointAmount); // Reserve memory upfront

		while(lasReader->read_point())
		{
			LASpoint &pointReader = lasReader->point;
			PointXYZI point;


			// get XYZ information
			point.x = static_cast<float>(pointReader.get_x() + offsetX);
			point.y = static_cast<float>(pointReader.get_y() + offsetY);
			point.z = static_cast<float>(pointReader.get_z() + offsetZ);

			// get Intensity information
			point.intensity = static_cast<float>(pointReader.get_intensity());


			cloud->points.push_back(point);
		}

        lasReader->close();
		delete lasReader;
		lasReader = nullptr;

        cloud->width = cloud->points.size();
        cloud->height = 1;
        cloud->is_dense = false;


        cerr << " Start downsampling..." << endl;
        PointCloud<PointXYZI>::Ptr downsample_cloud(new PointCloud<PointXYZI>);
        *downsample_cloud = *OctFilter(cloud, leaf_size, leaf_size, leaf_size);  // set your voxel size
        // VoxelGrid<PointXYZI> VGF;
        // VGF.setInputCloud(cloud);
        // VGF.setLeafSize(leaf_size, leaf_size, leaf_size);
        // VGF.filter(*downsample_cloud);
        cerr << " After downsmapling saved " << downsample_cloud->points.size() << "points"<< endl;


        cerr << " Start merging..." << endl;
        *merged_cloud += *downsample_cloud;

        cloud.reset();
		cloud = nullptr;
        downsample_cloud.reset();
        downsample_cloud = nullptr;

        cout << endl;

    }

    // Saving the point cloud
    stringstream ss;
    ss << leaf_size << "_" << output_file_name;
    writer.write<pcl::PointXYZI>(ss.str(), *merged_cloud, true);
    cerr << "Stored " << merged_cloud->points.size() << "points in " << ss.str() << endl;
    merged_cloud.reset();
	merged_cloud = nullptr;    

    return 0;

}

		    // while(lasReader->read_point())
		    // {
			//     LASpoint &pointReader = lasReader->point;
			//     PointXYZRGB point;

			//     point.x = pointReader.get_x() + offsetX;
			//     point.y = pointReader.get_y() + offsetY;
			//     point.z = pointReader.get_z() + offsetZ;
			//     // Extract RGB data. LAS stores RGB as 16 bits each, but PCL uses 8 bits each

            //     uint8_t r = static_cast<uint8_t>(pointReader.get_R() / 256);  // Assuming LAS stores RGB in [0, 65535]
            //     uint8_t g = static_cast<uint8_t>(pointReader.get_G() / 256);
            //     uint8_t b = static_cast<uint8_t>(pointReader.get_B() / 256);

            //     uint32_t rgb = (static_cast<uint32_t>(r) << 16 |
            //                     static_cast<uint32_t>(g) << 8 |
            //                     static_cast<uint32_t>(b));
            //     point.rgb = *reinterpret_cast<float*>(&rgb);


            //     // point.r = pointReader.get_R() >> 8; // Shift by 8 to convert from 16 bits to 8 bits
            //     // point.g = pointReader.get_G() >> 8; // Shift by 8 to convert from 16 bits to 8 bits
            //     // point.b = pointReader.get_B() >> 8; // Shift by 8 to convert from 16 bits to 8 bits

			//     cloud->points.push_back(point);
		    // }
