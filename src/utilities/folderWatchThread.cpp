//
//  folderWatchThread.cpp
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/9/13.
//
//

#include "folderWatchThread.h"

// the static event, or any static variable, must be initialized outside of the class definition.
ofEvent<ofDirectory> folderWatchThread::globalFolderNotification = ofEvent<ofDirectory>();

folderWatchThread::folderWatchThread() {
    dirPath = "~/";
    fileCount = 0;
    lastCount = 0;
    validDir = false;
    newFiles = false;
}

folderWatchThread::~folderWatchThread() {
    
}

void folderWatchThread::setPath(string path) {
    lock();
    if(ofDirectory::doesDirectoryExist(path, false)) {
        dirPath = path;
        dir.allowExt("png");
        dir.allowExt("jpg");
        dir.listDir(dirPath);
        dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
        fileCount = dir.size();
        validDir = true;
        ofLog(OF_LOG_NOTICE, "The directory <" + dirPath + "> DOES exist.");
        start();
    } else {
        ofLog(OF_LOG_ERROR, "The directory <" + path + "> does NOT exist.");
        validDir = false;
        stop();
    }
    unlock();
}

int folderWatchThread::getLastFileCount() {
    if(validDir) {
        return lastCount;
    } else {
        return -1;
    }
}

int folderWatchThread::getFileCount() {
    if(validDir) {
        return fileCount;
    } else {
        return -1;
    }
}

void folderWatchThread::start(){
    startThread(true, false);   // blocking, verbose
}

void folderWatchThread::stop(){
    stopThread();
}

//--------------------------
void folderWatchThread::threadedFunction(){
    
    while( isThreadRunning() != 0 ){
        if( lock() ){
            if(validDir) {
                dir.listDir(dirPath);
                dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
                fileCount = dir.size();
                if (fileCount != lastCount) {
                    newFiles = true;
//                    ofLog(OF_LOG_NOTICE, "::: LastCount: %d  fileCount: %d", lastCount, fileCount);
                }
                lastCount = fileCount;
            }
            unlock();
            if(newFiles) {
                // signal an event notification here
                ofNotifyEvent(folderChanged, dir, this);
                newFiles = false;
            }
            ofSleepMillis(1 * 500);
        }
    }
}
