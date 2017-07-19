# 说明
1. 更改js/cmdControl.js中的一些配置。（文件的开头）
``` javascript

// 小E开发板的uid
var devUid = '';
// 小E开发板的Appkey
var devAppkey = '';
// 控制端的uid
var userUid = '';
// 控制端的Secretkey
var userSecretkey = '';

```
2. 此页面为手机端页面，可以用hbuilder打包为apk安装到手机上，hbuilder打包很简单，具体百度。
3. smart_e_taste_v1.4文件夹下是esp8266的代码，需要烧写代码到开发板，才能控制gpio5，pwm控制的话，小e官方源固件就可以控制。