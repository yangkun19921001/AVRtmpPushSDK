//
// Created by 阳坤 on 2020-07-03.
//

#include "IThread.h"


void *_main(void *pVoid) {
    IThread *thread = static_cast<IThread *>(pVoid);
    //交于子类实现
    thread->main();
    return 0;
}


void IThread::start() {
    pthread_create(&this->pId, 0, _main, this);
}

void IThread::stop() {
//    pthread_join(pId,0);
}


