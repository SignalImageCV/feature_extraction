// ROS
#include <ros/ros.h>
// STL
#include <iostream>
// PCL
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/kdtree/kdtree.h>

#include <pcl/filters/conditional_removal.h>

#include <pcl/io/pcd_io.h>
#include <pcl/console/parse.h>

#include <pcl/segmentation/extract_clusters.h>
#include <pcl/segmentation/conditional_euclidean_clustering.h>

bool enforceIntensitySimilarity (const pcl::PointXYZI& point_a, const pcl::PointXYZI& point_b, float squared_distance)
{
  if (fabs (point_a.intensity - point_b.intensity) < 0.1f)
    return (true);
  else
    return (false);
}

class PclTest
{

    typedef pcl::PointXYZI Point;
    typedef pcl::PointCloud<Point> PointCloud;
    
  public:

    PclTest(char** argv);
    ~PclTest();

    // bool enforceIntensitySimilarity (const Point& point_a, const Point& point_b, float squared_distance);

  private:
    
    std::string pcdPath;

    void segmentCloud (const PointCloud::Ptr cloud, std::vector<pcl::PointIndices>& clusters);

};

PclTest::PclTest(char** argv)
{
  ros::NodeHandle nh("~");
  
  pcdPath = argv[1];

  PointCloud::Ptr cloud(new PointCloud);

  //////////////////////
  /* Load Point Cloud */
  //////////////////////
  pcl::io::loadPCDFile<Point>(pcdPath, *cloud);
  
  ///////////////////
  /* Segment Cloud */
  ///////////////////
  std::vector<pcl::PointIndices> clusters;

  segmentCloud(cloud,clusters);

}

PclTest::~PclTest(){}

void PclTest::segmentCloud (const PointCloud::Ptr cloud, std::vector<pcl::PointIndices>& clusters)
{

  pcl::ConditionalEuclideanClustering<Point> cec (true);
  cec.setInputCloud (cloud);
  
  // bool (*fptr) (const Point& point_a, const Point& point_b, float squared_distance) = &PclTest::enforceIntensitySimilarity;

  cec.setConditionFunction (&enforceIntensitySimilarity);
  /*
  // Points within this distance from one another are going to need to validate the enforceIntensitySimilarity function to be part of the same cluster:
  cec.setClusterTolerance (clusterTolerance);
  // Size constraints for the clusters:
  cec.setMinClusterSize (clusterMinCount);
  cec.setMaxClusterSize (clusterMaxCount);
  // The resulting clusters (an array of pointindices):
  cec.segment (*clusters);
  // The clusters that are too small or too large in size can also be extracted separately:
  // cec.getRemovedClusters (small_clusters, large_clusters);
  */

  /*
  // kd-tree object for searches.
  pcl::search::KdTree<Point>::Ptr kdtree(new pcl::search::KdTree<Point>);
  kdtree->setInputCloud(cloud);

  // Euclidean clustering object.
  pcl::EuclideanClusterExtraction<Point> clustering;

  // Set cluster tolerance to 2cm (small values may cause objects to be divided
  // in several clusters, whereas big values may join objects in a same cluster).
  clustering.setClusterTolerance(1.2f);
  // Set the minimum and maximum number of points that a cluster can have.
  clustering.setMinClusterSize(5);
  clustering.setMaxClusterSize(50);
  clustering.setSearchMethod(kdtree);
  clustering.setInputCloud(cloud);

  clustering.extract(clusters);

  int numClusters = clusters.size();
  std::cout << numClusters << std::endl;
  */
}

// bool PclTest::enforceIntensitySimilarity (const Point& point_a, const Point& point_b, float squared_distance)
// {
//   if (fabs (point_a.intensity - point_b.intensity) < 0.1f)
//     return (true);
//   else
//     return (false);
// }

int main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "pcl_test");

  PclTest node(argv);
  return 0;
}