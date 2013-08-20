//
//  folderWatchThread.h
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/9/13.
//
//

#ifndef __SUPRNRDY__folderWatchThread__
#define __SUPRNRDY__folderWatchThread__

#include "ofMain.h"

class folderWatchThread : public ofThread {
public:
    
    ofEvent<ofDirectory> folderChanged;                      // For each instance
    static ofEvent<ofDirectory> globalFolderNotification;    // Global notification
    
    folderWatchThread();
    ~folderWatchThread();
    
    void start();
    void stop();
    void threadedFunction();
    
    void setPath(string);
    
    int getFileCount();
    int getLastFileCount();

private:
    ofDirectory dir;
    string dirPath;
    int fileCount, lastCount;
    bool validDir;
    bool newFiles;
    
    
};

#endif /* defined(__SUPRNRDY__folderWatchThread__) */

