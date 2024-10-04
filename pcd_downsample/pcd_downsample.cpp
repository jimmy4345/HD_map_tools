#include <iostream>
#include <cstdlib>
//#include <liblas/liblas.hpp>
//#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <cstring>

using namespace std;

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
		if (strcmp(getExtension(dirp->d_name),"pcd") == 0)
		{
			files.push_back(string(dirp->d_name));
		}
    }
    closedir(dp);
    return 0;
}

template <class T>
pcl::PointXYZ getReferencePoint(const T input_cloud){
    pcl::PointXYZ ref_point;
    for(auto pt: input_cloud->points){
        //make sure the point is a valid point
        if(std::isfinite(pt.x) && std::isfinite(pt.y) && std::isfinite(pt.z)){
            ref_point.x = pt.x;
            ref_point.y = pt.y;
            ref_point.z = pt.z;
            break;
        }
    }
    return ref_point;
}

template <class T>
void translatePointCloud(const T input_cloud, T& output_cloud,const pcl::PointXYZ origin){
    output_cloud->points.clear();
    for(auto in_pt: input_cloud->points){
        auto out_pt = in_pt;
        out_pt.x -= origin.x;
        out_pt.y -= origin.y;
        out_pt.z -= origin.z;
        output_cloud->push_back(out_pt);
    }
    output_cloud->height = input_cloud->height;
    output_cloud->width = input_cloud->width;
}

int main (int argc, char *argv[])
{
	
	cout << "===================================================================" << endl;
	cout << "PCD down sample with leaf size can be chosen." << endl;
	cout << "Current for Ubuntu18.04 using liblas dev package(Ubuntu 20.04 can't use, need to check)" << endl;
	cout << "===================================================================" << endl;
	cout << endl;

	// cout << "Enter full .las file path: (or you can also drag the file here)" << endl;
    
	// getline(cin, filePath);
	// Edited to make fully command-line:
	string input_dir = string(argv[1]);
	string output_dir = string(argv[2]);
	float leaf_size = atof(argv[3]);
	string leafsize = string(argv[3]);
    vector<string> files = vector<string>();


	getdir(input_dir,files);


	//cout << files.size() << endl;   

	for (unsigned int i = 0;i < files.size();i++){
		//cout << files.size() << endl;
		string pcdfile_path;
		pcdfile_path = input_dir + "/" + files[i];

		std::cerr << "INFO : Loading : " << pcdfile_path << std::endl;


		// instancing a new PCL(XYZI) pointcloud object
    	pcl::PointCloud<pcl::PointXYZI>::Ptr cloud1 (new pcl::PointCloud<pcl::PointXYZI>);

		if (pcl::io::loadPCDFile<pcl::PointXYZI>(pcdfile_path.c_str(), *cloud1) == -1)
		{
			PCL_ERROR("Cloudn't read file!");
			return -1;
		}
		//pcl::io::loadPCDFile<pcl::PointXYZI>(pcdfile_path.c_str(), *cloud1);																																																																																																																																																																																																																																																																																																																						

		
		//pcl::io::loadPCDFile (pcdfile_path, *cloud1);															
  		std::cerr << "PointCloud before filtering: " << cloud1->width * cloud1->height 
       		<< " data points " << std::endl;

      	//translate pointcloud to local frame to avoid losing precision
      	pcl::PointCloud<pcl::PointXYZI>::Ptr translated_input_cloud1 (new pcl::PointCloud<pcl::PointXYZI>);
      	pcl::PointXYZ origin = getReferencePoint(cloud1);
      	translatePointCloud(cloud1, translated_input_cloud1, origin);

  		// Create the filtering object
		// instancing a new PCL(XYZI) pointcloud object after filterd
    	pcl::PointCloud<pcl::PointXYZI>::Ptr cloud1_filtered (new pcl::PointCloud<pcl::PointXYZI>);
  		pcl::VoxelGrid<pcl::PointXYZI> sor;
  		sor.setInputCloud (translated_input_cloud1);
  		sor.setLeafSize (leaf_size, leaf_size, leaf_size);
  		sor.filter (*cloud1_filtered);

		//translate pointcloud back to original frame
      	pcl::PointCloud<pcl::PointXYZI>::Ptr translated_filtered_cloud1 (new pcl::PointCloud<pcl::PointXYZI>);
      	pcl::PointXYZ inverse_origin;
      	inverse_origin.x = -origin.x;
      	inverse_origin.y = -origin.y;
      	inverse_origin.z = -origin.z;
      	translatePointCloud(cloud1_filtered, translated_filtered_cloud1, inverse_origin);





		// Allows output file to be set:
		pcl::io::savePCDFileBinary (output_dir + "/" + leafsize + "_" + files[i].replace(files[i].find("pcd"),3,"pcd"), *translated_filtered_cloud1);
  
		std::cerr << "Saved " << cloud1_filtered->points.size() << "points in "<< output_dir + "/" + leafsize + "_" + files[i] << std::endl;
		cout << endl;

	}

	return 0;
 
}
