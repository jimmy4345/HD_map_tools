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


/*template <class T>
void translatePointCloud(const T input_cloud, T& output_cloud, float x, float y, float z){
    output_cloud->points.clear();
    for(input_cloud->points){
        output_cloud->point. += x;
        out_pt.y += y;
        out_pt.z += z;
        output_cloud->push_back(out_pt);
    }
    output_cloud->height = input_cloud->height;
    output_cloud->width = input_cloud->width;
}*/

int main (int argc, char *argv[])
{
	
	cout << "===================================================================" << endl;
	cout << "PCD move with position can be changed" << endl;
	cout << "Current for Ubuntu18.04 using liblas dev package(Ubuntu 20.04 can't use, need to check)" << endl;
	cout << "===================================================================" << endl;
	cout << endl;

	// cout << "Enter full .las file path: (or you can also drag the file here)" << endl;
    
	// getline(cin, filePath);
	// Edited to make fully command-line:
	string input_dir = string(argv[1]);
	string output_dir = string(argv[2]);
	float x = atof(argv[3]);
	float y = atof(argv[4]);
	float z = atof(argv[5]);

	
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


		// Fill in the cloud data
		translated_input_cloud1->width    = cloud1->width;				// This means that the point cloud is "unorganized"
		translated_input_cloud1->height   = cloud1->height ;						// (i.e. not a depth map)
		translated_input_cloud1->is_dense = false;
		translated_input_cloud1->points.resize (cloud1->width * cloud1->height);

      	//pcl::PointXYZ origin = getReferencePoint(cloud1);
		int j = cloud1->width * cloud1->height;
		int k = 0;
		while(k < j){
			translated_input_cloud1->points[k].x =  cloud1->points[k].x + x;
			translated_input_cloud1->points[k].y =  cloud1->points[k].y + y;
			translated_input_cloud1->points[k].z =  cloud1->points[k].z + z;
			translated_input_cloud1->points[k].intensity =  cloud1->points[k].intensity;

			k++;
		}			


		// Allows output file to be set:
		pcl::io::savePCDFileASCII (output_dir + "/" + "move_" + files[i].replace(files[i].find("pcd"),3,"pcd"), *translated_input_cloud1);
  
		std::cerr << "Saved " << translated_input_cloud1->points.size() << "points in "<< output_dir + "/" + "move_" + files[i] << std::endl;
		cout << endl;

	}

	return 0;
 
}
