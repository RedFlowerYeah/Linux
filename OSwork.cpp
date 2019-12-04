#include<iostream>     //主要用于一些标注输入输出，例如cin和cout操作
#include<cstring>      //标准C++函数库，主要用于字符串的处理
#include<stdlib.h>     //一些宏和通用工具
#include<unistd.h>     //针对系统调用的封装
#include<string.h>      //定义各种操作字符数组
#include<sys/stat.h>    //文件状态
#include<dirent.h>      //文件操作函数
#include<fcntl.h>       //文件控制函数
#include<ftw.h>        //文件树漫游
using namespace std;

//显示当前所在目录的路径名
void pwd() {
    char path[100];
    getcwd(path,sizeof(path));//通过getcwd()获取文件路径
    cout<<"CurrentPath is： "<<path<<endl;
}


//列出指定目录名中的所有目录及文件
bool listFile(string dir) {
    DIR* d = opendir(dir.c_str());  //打开文件目录
    if(d==NULL) {
        return false;
    } else {
	struct dirent *dirent;//dirent结构体，文件进行操作
        while(dirent=readdir(d)) {//对文件进行读操作
	    cout<<endl;
	    cout<<"  "<<dirent->d_name<<endl;	//dirent指向结构体中的d_name,也就是文件名
	    cout<<endl;
	}
	closedir(d);
    return true;
	}
}

//进入其他目录
void cd()
{
   char pathname[50];
   cin>>pathname;
   if(chdir(pathname) == -1)
   //chdir函数用于改变当前工作目录。调用参数是指向目录的指针，调用进程需要有搜索整个目录的权限。每个进程都具有一个当前工作目录
   {
      cout<<"this path is not exist"<<endl;
 
    }
    else
    {
      cout<<"change path success"<<endl;
     }
}

//新建目录
bool mkdirDirectory(string dir) {
    if(mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==0) {	//int mkdir(const char *pathname, mode_t mode);mkdir形式
	/*S_IRWXU表示文件拥有者拥有读写和执行操作的权限，
	S_IRWXG用户组拥有读，写和执行操作的权限，
	S_IROTH其他用户拥有可读的权限，
	S_IXOTH其他用户拥有可写的权限。
	若目录创建成功，则返回true，否则则返回false。*/
        return true;
    } else {
        return false;
	}
}

//删除目录
int removeDirectory(const char *dir)//返回的是一个指针类型的数组
{
	//文件目录以及文件上一级目录
	char cur_dir[] = ".";
	char up_dir[] = "..";
	char dir_name[128];
	DIR *dirp;
	struct dirent *dp;//dirent结构体，对函数进行操作
	struct stat dir_stat;

	// 参数传递进来的目录不存在，直接返回
	if ( 0 != access(dir, F_OK) ) {//F_OK是判断目录/文件是否存在
		return 0;
	}

	// 获取目录属性失败，返回错误
	if ( 0 > stat(dir, &dir_stat) ) {
		perror("get directory stat error");
		return -1;
	}

	if ( S_ISREG(dir_stat.st_mode) ) {	// S_ISREG是判断是否是一个常规文件，st_mode访问的是权限，普通文件直接删除
		remove(dir);
	} else if ( S_ISDIR(dir_stat.st_mode) ) {	// S_ISDIR判断是否是一个目录，st_mode访问的是权限，目录文件，递归删除目录中内容
		dirp = opendir(dir);
		while ( (dp=readdir(dirp)) != NULL ) {//readdir是读取目录文件函数，如果目录函数中有函数，则要进行删除文件操作
			if ( (0 == strcmp(cur_dir, dp->d_name)) || (0 == strcmp(up_dir, dp->d_name)) ) {
				/*strcmp是比较函数，比较两个函数的名字，忽略.和..*/
				continue;
			}
			sprintf(dir_name, "%s/%s", dir, dp->d_name);
			removeDirectory(dir_name);   // 递归调用
		}
		closedir(dirp);

		
		rmdir(dir);		// 删除空目录
	} else {
		perror("unknow file type!");	
	}
	
	return 0;
}

//目录菜单
void menu() {
    system("clear");
    cout<<"*********************************************"<<endl;
	cout<<"  Linux简单命令解析器"<<endl;
	cout<<"  Please input you commands:"<<endl;
    cout<<"  pwd(查看当前路径)"<<endl;
    cout<<"  list<目录名>(列出指定目录名中的所有目录及文件)"<<endl;
    cout<<"  cd<目录名>(改变路径)"<<endl;
    cout<<"  mkdir<目录名>"<<endl;
    cout<<"  rmdir<目录名>"<<endl;
	cout<<"  exit(退出解析器)"<<endl;
    cout<<"*********************************************"<<endl;
    cout<<endl;
}

int main() {
    menu();
    string str;
    while(str!="exit") {
		cin>>str;
		if (str=="pwd")
		{
			menu();
			pwd();
		}
		if (str=="list")
		{
			menu();
			cout<<"Please enter a specified directory name:";
            string dir;
            cin>>dir;
            if(!listFile(dir)) {
 	        cout<<"Failed to open or do not exist in this directory!!"<<endl;
            }
		}
		if (str=="cd")
		{
			menu();
			cout<<"Please enter your want to cd's directory name:";
			cd();
			pwd();
		}
		if (str=="mkdir")
		{
			menu();
			cout<<"Please enter a new directory name:";
			string dir;
			cin>>dir;
			if(!mkdirDirectory(dir)) {
				cout<<"New directory is exist!"<<endl;
				}
				else {
					cout<<"Directory created successfully"<<endl;
					}
		}
		if (str=="rmdir")
		{	
			menu();
			cout<<"Please enter your delete directory name:";
			char *dir=new char[100];
			cin>>dir;
			if(removeDirectory(dir)==-1){
				cout<<"this directory is not exist!"<<endl;
			}
			else{
				removeDirectory(dir);
				cout<<"the directory rmdir success!"<<endl;
			}
		}
    }
	return 0;
}