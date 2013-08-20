//
//  imageEngine.cpp
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/9/13.
//
//

#include "imageEngine.h"

imageEngine::imageEngine() {
    ofAddListener(watchThisFolder.folderChanged, this, &imageEngine::onFolderUpdated);
    watchThisFolder.setPath("/Users/brizo/Desktop/images");
}

imageEngine::~imageEngine() {
    }


void imageEngine::onFolderUpdated(ofDirectory &e) {
    watchThisFolder.lock();
    int tempnum = watchThisFolder.getFileCount();
    
    for(int i = 0; i < (int)e.size(); i++){
		string fileInfo = "file " + ofToString(i + 1) + " = " + e.getName(i);
        //ofLog(OF_LOG_NOTICE, "file: " + fileInfo);
        // e.getPath(i);
        string tempPath = e.getPath(i);
        if (!dirListing.nodeExists(tempPath)) {
            dirListing.push(tempPath);
            imageQueue.push(tempPath);
        }
	}
    watchThisFolder.unlock();
    ofLog(OF_LOG_NOTICE, "files before clean: " + ofToString(dirListing.size()));
    dirListing.cleanList();
    imageQueue.cleanList();
    

    string fileCount = ofToString(tempnum) + " Files ";
    ofLog(OF_LOG_NOTICE, "Event Notification of new files: " + ofToString(dirListing.size()));
}

string imageEngine::getImagePath() {
    return imageQueue.pop();
}