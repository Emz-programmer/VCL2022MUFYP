/*
Visual Campus Locator
Emily Hynes
Maynooth University FYP
*/

#include <iostream>
#include <vector>
#include <stdio.h>
#include <experimental/filesystem>
#include <restbed>
#include <sqlite3.h>
// DBoW2
#include "DBoW2.h"
// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
using namespace DBoW2;
using namespace std;
using namespace cv;
using namespace restbed;
void loadFeatures(vector<vector<cv::Mat > > &features, cv::Mat image);
void changeStructure(const cv::Mat &plain, vector<cv::Mat> &out);
std::string b64decode(const void* data, const size_t len);
QueryResults queryDatabase(const vector<vector<cv::Mat > > &features);
int NIMAGES = 1;
double confidenceScore=0;
string buildingNames []= {"Arts", "Iontas", "Engineering", "Eolas", "John Hume"};

int getMatch(QueryResults ret){
  int res=0;
  double buildingScores []= {0,0,0,0,0};
  double idCount[] = {0,0,0,0,0};
  for(int i=0;i<5;i++){
    int idNum = ret[i].Id/40;
    idCount[idNum]++;
    buildingScores[idNum] +=ret[i].Score;
  }
  double maxScore=0;
  for(int i=0; i<5;i++){
    if(buildingScores[i]>maxScore){
      maxScore = buildingScores[i];
      confidenceScore = buildingScores[i]/idCount[i];
      res = i;
    }
  }
  return res;
}

string getRecall(QueryResults ret){
  string results="";
  for(int i=0; i<5;i++){
    results+=buildingNames[ret[i].Id/40]+",";
  }

  return results;
}

void post_method_handler( const shared_ptr< Session > session )
{  
    const auto request = session->get_request( );
    size_t content_length = 0;
    request->get_header( "Content-Length", content_length );
    cout << "Post msg handler\n";
    session->fetch( content_length, [ ]( const shared_ptr< Session > session, const Bytes & body )
    {
        char *str = new char[body.size() + 1];
        memcpy(str, body.data(), body.size());
        str[body.size()] = 0;
        string decodedImg= b64decode(str, body.size());
        std::vector<uchar> data(decodedImg.begin(), decodedImg.end());
        Mat img = cv::imdecode(cv::Mat(data), 1);
        
        vector<vector<cv::Mat > > features;
    
        loadFeatures(features, img);

        QueryResults ret = queryDatabase(features);
        int result = getMatch(ret);
        string recall = getRecall(ret);
        string ans = buildingNames[result]+ ","+to_string(confidenceScore)+","+recall; 
        
        //cout << "Input\n" << str << endl;
        session->close( OK, ans);
    } );
}

int main(){
    auto resource = make_shared< Resource >( );
    resource->set_path( "/2022fypVCL" );
    resource->set_method_handler( "POST", post_method_handler );

    auto settings = make_shared< Settings >( );
    settings->set_port( 2022 );
    settings->set_default_header( "Connection", "close" );
    settings->set_default_header( "Access-Control-Allow-Origin", "*");

    Service service;
    service.publish( resource );
    service.start( settings );

    return 0;
}

void loadFeatures(vector<vector<cv::Mat > > &features, cv::Mat image)
{
  features.clear();
  features.reserve(NIMAGES);
  cv::Ptr<cv::ORB> orb = cv::ORB::create();     
  cv::Mat mask;
  vector<cv::KeyPoint> keypoints;
  cv::Mat descriptors;
  orb->detectAndCompute(image, mask, keypoints, descriptors);
  features.push_back(vector<cv::Mat >());
  changeStructure(descriptors, features.back());  
}

void changeStructure(const cv::Mat &plain, vector<cv::Mat> &out)
{
  out.resize(plain.rows);
  for(int i = 0; i < plain.rows; ++i)
  {
    out[i] = plain.row(i);
  }
}

QueryResults queryDatabase(const vector<vector<cv::Mat > > &features)
{
  OrbDatabase db("vcl_db.yml.gz");  
  cout << "Querying image: " << endl;  
  QueryResults ret;  
  db.query(features[0], ret, 6);
  cout << "Searching for Image " << 0 << ". " << ret << endl;  
  cout << endl;
  return ret;
}
// Base64 Converter from: https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
static const int B64index[256] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

std::string b64decode(const void* data, const size_t len)
{
    unsigned char* p = (unsigned char*)data;
    int pad = len > 0 && (len % 4 || p[len - 1] == '=');
    const size_t L = ((len + 3) / 4 - pad) * 4;
    std::string str(L / 4 * 3 + pad, '\0');

    for (size_t i = 0, j = 0; i < L; i += 4)
    {
        int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
        str[j++] = n >> 16;
        str[j++] = n >> 8 & 0xFF;
        str[j++] = n & 0xFF;
    }
    if (pad)
    {
        int n = B64index[p[L]] << 18 | B64index[p[L + 1]] << 12;
        str[str.size() - 1] = n >> 16;

        if (len > L + 2 && p[L + 2] != '=')
        {
            n |= B64index[p[L + 2]] << 6;
            str.push_back(n >> 8 & 0xFF);
        }
    }
    return str;
}
