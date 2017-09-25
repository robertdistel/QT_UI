#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

template<class T>
class SharedMemory
{
public:
    inline T* operator->(){return p;}
    inline T& operator*(){return *p;}

    SharedMemory():fd(0),p(nullptr)
    {
    }

    SharedMemory(const char* filename, bool erase=false)
    {
    	if (erase)
    		unlink(filename); //delete it if it already exists - forcing restart from a fresh state

        fd = open(filename, O_RDWR , S_IRUSR | S_IWUSR);
        if (fd==-1)
        {
            //file doesnt exists - open it and fill it with zeros to the write size
            fd = open(filename, O_RDWR|O_CREAT|O_APPEND , S_IRUSR | S_IWUSR);

            if (fd==-1)
            {
                perror(filename);
                exit(-1);
            }

            uint8_t b(0);
            for (size_t k=0; k<sizeof(T); k++)
            {
                if (write(fd,&b,1)!=1)
                {
                    perror(0);
                    exit(-1);
                }
            }
            syncfs(fd);

        }

        // Our memory buffer will be readable and writable:
        int protection = PROT_READ | PROT_WRITE;

        // The buffer will be shared (meaning other processes can access it), but
        // anonymous (meaning third-party processes cannot obtain an address for it),
        // so only this process and its children will be able to use it:
        int visibility =  MAP_SHARED;

        // The remaining parameters to `mmap()` are not important for this use case,
        // but the manpage for `mmap` explains their purpose.
        auto pp=mmap(NULL, sizeof(T), protection, visibility, fd, 0);
        p = static_cast<T*>(pp);
    }
    ~SharedMemory()
    {
        if (p)
            munmap(p,sizeof(T));
        p=nullptr;
        close(fd);
        fd=0;
    }
private:
    int fd;
    T* p;

};





#endif // SHAREDMEMORY_H
