#include <iostream>
#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include<filesystem>
#include "DBoW2.h" 
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

using namespace DBoW2;
using namespace std;
namespace fs = std::filesystem;

void loadFeatures(vector<vector<cv::Mat > > &features, string path);
void changeStructure(const cv::Mat &plain, vector<cv::Mat> &out);
void vocabCreation(const vector<vector<cv::Mat > > &features);
void createDatabase(const vector<vector<cv::Mat > > &features);

int NIMAGES=0;

void getImageCount(string imgPath){
    int count=0;
    for (auto& p : fs::directory_iterator(imgPath)){
        count++;

    }
    NIMAGES = count;
}
int main(){    
    vector<vector<cv::Mat > > features;
    string path;
    path="./images/training";
    getImageCount(path+"/");
    loadFeatures(features, path);
    vocabCreation(features);
    createDatabase(features);    
    return 0;
}

void loadFeatures(vector<vector<cv::Mat > > &features, string path){
    features.clear();
    features.reserve(NIMAGES);

    cv::Ptr<cv::ORB> orb = cv::ORB::create();

    cout << "Extracting ORB features..." << endl;
    for(int i = 1; i <= NIMAGES; ++i)
    {
        stringstream ss;
        ss << path << "/training" << i << ".jpg";
        cv::Mat image = cv::imread(ss.str(), 0);
        cv::Mat mask;
        vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;
        orb->detectAndCompute(image, mask, keypoints, descriptors);
        features.push_back(vector<cv::Mat >());
        changeStructure(descriptors, features.back());
    }
}

void changeStructure(const cv::Mat &plain, vector<cv::Mat> &out)
{
    out.resize(plain.rows);
    for(int i = 0; i < plain.rows; ++i)
    {
        out[i] = plain.row(i);
    }
}

void vocabCreation(const vector<vector<cv::Mat > > &features)
{
    // branching factor and depth levels 
    const int k = 8;
    const int L = 3;
    const WeightingType weight = TF_IDF;
    const ScoringType scoring = BHATTACHARYYA;
    OrbVocabulary voc(k, L, weight, scoring);
    cout << "Creating a small " << k << "^" << L << " vocabulary..." << endl;
    voc.create(features);
    cout << endl << "Saving vocabulary..." << endl;
    voc.save("vcl_voc.yml.gz");
    cout << "Vocabulary saved!" << endl;
}

void createDatabase(const vector<vector<cv::Mat > > &features)
{
  OrbVocabulary voc("vcl_voc.yml.gz");
  
  OrbDatabase db(voc, false, 0);
  for(int i = 0; i < NIMAGES; i++)
  {
    db.add(features[i]);
  }
  QueryResults ret;
  for(int i = 0; i < NIMAGES; i++)
  {
    db.query(features[i], ret, 5);

    cout << "Validating database " << i << ". " << ret << endl;
  }
  cout << endl;
  cout << "Saving database..." << endl;
  db.save("vcl_db.yml.gz");
  cout << "Database saved!" << endl;
}
