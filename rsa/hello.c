#include <linux/kernel.h>

asmlinkage long sys_hello(void) {
	printk("Hello World\n");
	return 0;
}

asmlinkage long __x64_sys_hello(void) {
	printk("Hello world x64\n");
	return 0;
}
