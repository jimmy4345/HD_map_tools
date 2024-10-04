#include "tinyxml2.h"
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <cstring>

using namespace std;
using namespace tinyxml2;

// tinyxml2::XMLDocument doc;
//   doc.LoadFile(osm_file.c_str());
// tinyxml2::XMLElement* root = doc.FirstChildElement("osm");
//   if (root == NULL)
//   {
//     std::cerr << "\033[31;1mError: OSM NULL\033[m" << std::endl;
//     exit(1);
//   }

//   for (tinyxml2::XMLElement* element = root->FirstChildElement("node"); element != NULL; element = element->NextSiblingElement("node"))
//   {
//     LL2Point p;
//     double x, y;
//     for (tinyxml2::XMLElement* tag = element->FirstChildElement("tag"); tag != NULL; tag = tag->NextSiblingElement("tag"))
//     {
//       if (std::string(tag->Attribute("k")) == std::string("local_x")) p.local_x = tag->DoubleAttribute("v");
//       if (std::string(tag->Attribute("k")) == std::string("local_y")) p.local_y = tag->DoubleAttribute("v");
//       if (std::string(tag->Attribute("k")) == std::string("ele")) p.elevation = tag->DoubleAttribute("v");
//     }

//     p.id = element->UnsignedAttribute("id");
//     p.lat = element->DoubleAttribute("lat");
//     p.lon = element->DoubleAttribute("lon");

//     p.valid = true;

//     ll2_points_.addNewPoint(p);
//   }

int main (int argc, char *argv[]){

    string osm_file = string(argv[1]);
    string output_osm_file = string(argv[2]);
    double x_offset = atof(argv[3]);
	double y_offset = atof(argv[4]);
	double ele_offset = atof(argv[5]);

    XMLDocument doc;
    doc.LoadFile(osm_file.c_str());

    XMLElement* root = doc.FirstChildElement("osm");
    if(root == NULL){
        cerr << "\033[31;1mError: OSM NULL\033[m" << endl;
        exit(1);
    }

    for (XMLElement* element = root->FirstChildElement("node"); element != NULL; element = element->NextSiblingElement("node")){
        // LL2Point p;
        int a=0;
        double local_x=0, local_y=0, ele = 0;
        double new_local_x=0, new_local_y=0, new_ele = 0;
        for (XMLElement* tag = element->FirstChildElement("tag"); tag != NULL; tag = tag->NextSiblingElement("tag")){
            if (string(tag->Attribute("k")) == string("local_x")){
                local_x = tag->DoubleAttribute("v");
                new_local_x = local_x + x_offset;
                tag->SetAttribute("v", new_local_x);
            } 
            if (string(tag->Attribute("k")) == string("local_y")){
                local_y = tag->DoubleAttribute("v");
                new_local_y = local_y + y_offset;
                tag->SetAttribute("v",new_local_y);
            } 
            if (string(tag->Attribute("k")) == std::string("ele")){
                ele = tag->DoubleAttribute("v");
                new_ele = ele + ele_offset;
                tag->SetAttribute("v", new_ele);
            } 

            
            // if (string(tag->Attribute("k")) == string("ele")) p.elevation = tag->DoubleAttribute("v");
        }

        printf("local_x = %.3f , local_y = %.3f, elevation = %.3f\n",local_x,local_y,ele);
        printf("new_local_x = %.3f , new_local_y = %.3f, new_elevation = %.3f\n",new_local_x,new_local_y,new_ele);

        // for (XMLElement* tag1 = element->FirstChildElement("tag"); tag1 != NULL; tag1 = tag1->NextSiblingElement("tag")){
        //     if (string(tag1->Attribute("k")) == string("local_x")) 
                
        //         tag1->SetAttribute("v", new_local_x);
        //     if (string(tag1->Attribute("k")) == string("local_y")) 

        //         // tag->SetAttribute("v", new_local_y);
        //     if (string(tag1->Attribute("k")) == std::string("ele")) 

        //         // tag->SetAttribute("v", new_ele);
            
        //     // if (string(tag->Attribute("k")) == string("ele")) p.elevation = tag->DoubleAttribute("v");
        // }       


        
    

        // p.id = element->UnsignedAttribute("id");
        // p.lat = element->DoubleAttribute("lat");
        // p.lon = element->DoubleAttribute("lon");

        // p.valid = true;

        // ll2_points_.addNewPoint(p);
    }
    doc.SaveFile(output_osm_file.c_str());


    return 0;
}
