#include <iostream>
#include <cstdlib>
//#include <liblas/liblas.hpp>
//#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
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


//pcl::PointCloud<pcl::PointXYZI> *cloud1 = new pcl::PointCloud<pcl::PointXYZI>;




int main(int argc, char *argv[])
{
    string input_dir = string(argv[1]);
	//string output_dir = string(argv[2]);
    vector<string> files = vector<string>();


	getdir(input_dir,files);

    pcl::PointCloud<pcl::PointXYZI> *cloud1_add = new pcl::PointCloud<pcl::PointXYZI>;

    for (unsigned int i = 0;i < files.size();i++){
		//cout << files.size() << endl;
		string pcd_path;
		pcd_path = input_dir + "/" + files[i];


		// instancing a new PCL(XYZI) pointcloud object
    	pcl::PointCloud<pcl::PointXYZI> *cloud1 = new pcl::PointCloud<pcl::PointXYZI>;
        
        pcl::io::loadPCDFile(pcd_path.c_str(), *cloud1);
        *cloud1_add = *cloud1_add + *cloud1;



		delete cloud1;
	}

    // Allows output file to be set:
	pcl::io::savePCDFileBinary("test_pcd.pcd", *cloud1_add);
    return 0;
}
