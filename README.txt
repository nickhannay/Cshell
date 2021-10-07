

1.) The built-in print function accepts an empty argument.

2.) If multiple environment varibles are set on one line, and an error occurs, an error message will appear but,
    the valid environment variables will still be set.

3.) cshell still logs unkown commands, but with an error code

4.) All of the functions that are present in cshell.c can be found in either: log.c, envVar.c or built-in.c


Question 2:

I spent about 6 days on the assignment in total. However, most of the time was spent compiling the kernel for the bonus question. I learned quite a lot about inter proccess communication and how
Linux handles I/O. I also learned a lot about the kernel compilation process and how linux implements system calls.


------------- BONUS #1 ----------------------

linux kernel 5.10.18, available at: https://www.kernel.org/

required packages - libelf-dev, libssl-dev, libncurses-dev, bison, flex

Steps to add system call and compile kernel (x86_64 based architecture):

1) unpack the tar archive into /usr/src


2) Add the uppercase system call to the end of linux-5.10.18/arch/x86/entry/syscalls/syscall_64.tbl
		
	<next available#> 64	uppercase sys_uppercase		


3) Add the directory of the uppercase C source code to the kernel Makefile


4) NOTE: No need to define uppercase in syscalls.h as "__x64_sys_*() stubs are created on-the-fly for sys_*() system calls"


5) In the root directory of the kernel source run:

	sudo make menuconfig
		
		-exit configuration

	sudo make -jn 
		
		- n = ammount of cpu cores available to speed it up

	sudo make modules_install install
		
		- this should install the necessary files into /boot


6) Reboot the computer and select the new kernel in the grub menu 




------------- BONUS #2 --------------------

For the second bonus I decided to add a lowercase syscall. I felt that since I added the uppercase syscall, it would only make sense to add a lowercase syscall and it would fit well into the cshell.



All the steps to add the lowercase syscall are the same as #1, just substitute "uppercase" with "lowercase"
