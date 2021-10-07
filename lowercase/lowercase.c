#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>

asmlinkage long  __x64_sys_lowercase( const char* input , char* dest, int length){
	int i = 0;
	char buf[length];
	const long MAX_BUFFER = 255;
	long toUser;
	
	printk("sys_lowercase called\n");
	
	// copy from user space
	if(!strncpy_from_user(buf, input, length)){
		printk("copying from user space worked\n");
		printk("inputted string: %s\n", buf);
		while ( i < length){
			if(buf[i] >= 97 &&  buf[i] <= 122 ){
				buf[i] += 32;
			}
			i++;
		}

		printk("modified lowercase string:%s\n",buf);
		
		// check if <dest> pointer is valid user space address
		if(access_ok( dest, length) != 0){
			printk("destination is vald\n");
			// copy to modified string to user space
			toUser = copy_to_user(dest, buf, length);
			if(toUser == 0 )
				printk("copying to user space worked!\n");
		}
	}
	
	return 0;
}