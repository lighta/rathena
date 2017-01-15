/* 
 * File:   c_Module.cpp
 * Author: lighta
 * 
 * Created on January 15, 2017, 1:25 PM
 */

#include <map>
#include <mutex>
#include <vector>
#include <future> //async

#include "c_Module.h"
        

namespace ra {
  namespace common_new {
    struct c_Module::pImpl 
    {
      std::mutex aListenerMutex; //protec the listener map
      std::map<size_t,c_Module*> aListener;
      
      pImpl(){}
    };
    
    c_Module::c_Module() : aPimpl(new c_Module::pImpl){
    }

    c_Module::c_Module(const c_Module& orig) {
    }

    c_Module::~c_Module() {
    }

    void c_Module::notifyAll(size_t pKeyEvent) {
      std::vector<std::shared_future<void>> aFutureVect;
      { //lock scope
        std::lock_guard<std::mutex> _(aPimpl->aListenerMutex);
        for(auto lCurListener : aPimpl->aListener) {//launch all update in same time
          auto lFunc = [&]{ lCurListener.second->update(pKeyEvent); }; //small lambda to call
          aFutureVect.push_back(std::async(std::launch::async,lFunc).share());
        }
      }
      //now ensure they all finished
      for(auto lFut : aFutureVect){
        lFut.get();
      }
    }
    
    size_t c_Module::mAttach(c_Module* pModule) {
      std::lock_guard<std::mutex> _(aPimpl->aListenerMutex);
      size_t lKey=aPimpl->aListener.size();
      aPimpl->aListener[lKey] = pModule;
      return lKey;
    }

    void c_Module::mDettach(size_t pKey) {
      std::lock_guard<std::mutex> _(aPimpl->aListenerMutex);
      aPimpl->aListener.erase(pKey);
    }
  }
}