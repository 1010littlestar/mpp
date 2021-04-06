liteos下使用mpp下tools的方法：

1、在mpp目录下执行cp cfg.mak.single cfg.mak，使用cfg.mak.single覆盖cfg.mak
mpp# cp cfg.mak.single cfg.mak

2、在mpp/tools目录下执行make，生成libtools.a，将libtools.a拷贝到out/liteos/single/lib
mpp/tools# cp libtools.a ../out/liteos/single/lib/

3、在mpp/sample/liteos/app_init.c中app_init函数的末尾中添加一行tools_cmd_register();添加后的结果如下：
void app_init(void)
{
    ...
    SDK_init();
    sample_command();
    tools_cmd_register(); // add tools command
    CatLogShell();
    ...
}

4、将sample的mpp/liteos.mak中的tools库引用打开（默认是注释掉的），
打开mpp/liteos.mak中SDK_LIB里面-ltools前面的注释（删除#即可）

5、进入mpp/sample目录执行：
mpp/sample#make liteclean;make lite
这样，mpp/sample目录下面各子目录中的bin文件里就带有tools的功能。



How to use mpp tools on LiteOS:

step 1, Excute the command "cp cfg.mak.single cfg.mak" in the mpp directory.
mpp# cp cfg.mak.single cfg.mak

step 2, Execute "make" command in the mpp/tools directory to make libtools.a, copy libtools.a to out/liteos/single/lib directory.
mpp/tools# cp libtools.a ../out/liteos/single/lib/

step 3, Add function tools_cmd_register() to the end of the app_init function in the mpp/sample/liteos/app_init.c, as follows:
void app_init (void)
{
    ...
    SDK_init ();
    sample_command ();
    tools_cmd_register (); // add tools command
    CatLogShell ();
    ...
}

step 4, Add the tools library file to the sample mpp/liteos.mak file (default is commented),
Uncomment -ltools in SDK_LIB (delete #)

5, in the mpp/sample directory:
mpp/sample # make liteclean; make lite
In this way,  the tools functions are added to the bin file in the sub-directory of mpp/sample.
