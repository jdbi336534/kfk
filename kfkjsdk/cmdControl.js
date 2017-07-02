// 一些配置参数
// 小E开发板的uid
var  devUid = 'Fc5wGsTuvumomVom7eixwJ2UfKJwuxgvKf';
// 小E开发板的Appkey
var devAppkey='a56ae6d4-946d-880526';
// 控制端的uid
var userUid= 'Fc5wGsTuvumvyVvyFG7Tt9ShD6eFhTCBfw';
// 控制端的Secretkey
var userSecretkey='0b5cd6a7769618139b5593a1975ffc1e';
//负载均衡服务器地址.IM WEB地址
var balance_server = 'ws://lb.kaifakuai.com:1885';

var devState = 0;
var opt = {
        userid:userUid,
        appkey: devAppkey,
        secretkey: userSecretkey,
        balanceServer: balance_server,
        clean: 0
    }
    chatHistory = "";
    //事件回调函数
    function onClientEvent(event) {
    	//alert("IM注册触发.....");
    	console.log("回调对象->event 【"+JSON.stringify(event));
    	var num=0
    	console.log(num++)
        if (event.type === 'message') {
        	console.log("主题： "+event.topic+" 离在线状态："+event.status+"时间： "+ new Date().toLocaleTimeString()+" ms");
            if (event.topic === 'chat') {
                //收到及时消息
                var msg = event.message;
                if (((msg[0]) == 255) && ((msg[1]) == 255)) {
                    // rollBottom();
                    if (msg[6] == 16) {
                        // 请求成功
                        if (msg[7] == 0) {
                        }
                        //代表BCC校验错误
                        else if (msg[7] == 1) {
                        }
                        //失败
                        else if (msg[7] == 2) {
                        }
                        //模式错误
                        else if (msg[7] == 3) {
                            alert("请将换档开关调节到3档位，然后重启设备");
                        }
                        //代表指令非法
                        else if (msg[7] == 255) {
                        }
                    }
                    // 查询温度和湿度
                    else if (msg[6] == 32) {
                        // 请求成功
                        if (msg[7] == 0) {
                            //湿度整数部分
                            var humidity1 = msg[8];
                            //湿度小数部分
                            var humidity2 = msg[9];
                            //温度整数部分
                            var temperature1 = msg[10];
                            //温度小数部分
                            var temperature2 = msg[11];
                            humidity1 = humidity1.toString(16);
                            humidity2 = humidity2.toString(16);
                            temperature1 = temperature1.toString(16);
                            temperature2 = temperature2.toString(16);
                            var userNick = "小e";
                            var image = "./images/xiaoe.png";
                            var msg1 = "当前湿度：" + humidity1 + "." + humidity2
                                    + "%";
                            var msg2 = "当前温度：" + temperature1 + "."
                                    + temperature2 + "°C";
                            var msg = '<div ng-switch-default="" class="message you" >\
	                              <img class="avatar" src="' + image + '" title="' + userNick + '">\
	                            <div class="content">\
	                            <h4 class="nickname" >'
                                    + userNick
                                    + '</h4>\
	                            <div class="bubble js_message_bubble bubble_default left">\
	                            <div class="bubble_cont">\
	                            <div class="plain">\
	                            <pre class="js_message_plain">'
                                    + msg2
                                    + '<br>'
                                    + msg1
                                    + '</pre>\
	                            </div>\
	                            </div>\
	                            </div>\
	                            </div>\
	                            </div><div style="clear: both"></div>';
                            if (document.getElementById('panelBody-5')) {
                                document.getElementById('panelBody-5').innerHTML += msg;
                            }
                            // changeWindow();
                            // rollBottom();
                        }
                        //代表BCC校验错误
                        else if (msg[7] == 1) {
                        }
                        //失败
                        else if (msg[7] == 2) {
                            errorPositionMsg("失败");
                        }
                        //模式错误
                        else if (msg[7] == 3) {
                            errorPositionMsg("请将换档开关调节到1档位，然后重启设备");
                        }
                        //代表指令非法
                        else if (msg[7] == 255) {
                            errorPositionMsg("指令非法");
                        }
                    }
                    // 查询气压和海拔
                    else if (msg[6] == 48) {
                        // 请求成功
                        if (msg[7] == 0) {
                            var airPressure1 = msg[8];
                            var airPressure2 = msg[9];
                            var airPressure3 = msg[10];
                            var airPressure4 = msg[11];
                            var airPressure5 = msg[12];
                            airPressure1 = innerToString(airPressure1.toString(16));
                            airPressure2 = innerToString(airPressure2.toString(16));
                            airPressure3 = innerToString(airPressure3.toString(16));
                            airPressure4 = innerToString(airPressure4.toString(16));
                            var baseAirPressure = ((parseInt(airPressure5.toString(16)) - 5 - 2 + 237.15) / (15 + 237.15)) * 101325;
                            var airPressure = airPressure1 + airPressure2 + airPressure3 + airPressure4;
                            var airPressure22 = parseInt(airPressure);
                            airPressure = 44330 * (1 - Math.pow(airPressure22 / baseAirPressure, 1 / 5.255));
                            var userNick = "小e";
                            var image = "./images/xiaoe.png";
                            var msg1 = "当前气压：" + airPressure22 + "Pa";
                            var msg2 = "当前海拔：" + parseInt(airPressure) + "m";
                            var msg = '<div ng-switch-default="" class="message you" >\
	                            <img class="avatar" src="' + image + '" title="' + userNick + '">\
	                            <div class="content">\
	                            <h4 class="nickname" >' + userNick + '</h4>\
	                            <div class="bubble js_message_bubble bubble_default left">\
	                            <div class="bubble_cont">\
	                            <div class="plain">\
	                            <pre class="js_message_plain">' + msg1 + '<br>' + msg2 + '</pre>\
	                            </div>\
	                            </div>\
	                            </div>\
	                            </div>\
	                            </div><div style="clear: both"></div>';
                            if (document.getElementById('panelBody-5')) {
                                document.getElementById('panelBody-5').innerHTML += msg;
                            }
                            // changeWindow();
                            // rollBottom();
                        }
                        //代表BCC校验错误
                        else if (msg[7] == 1) {
                        }
                        //失败
                        else if (msg[7] == 2) {
                            errorPositionMsg("请检查气压传感器部分是否插上了跳线帽");
                        }
                        //模式错误
                        else if (msg[7] == 3) {
                            errorPositionMsg("请将换档开关调节到2档位，然后重启设备");
                        }
                        //代表指令非法
                        else if (msg[7] == 255) {
                        }

                    }// 可视交互
                    else if (msg[6] == 64) {
                        // 请求成功
                        if (msg[7] == 0) {
                            displayEMsg2(devUid, "发送成功");
                        }
                        //代表BCC校验错误
                        else if (msg[7] == 1) {
                        }
                        //失败
                        else if (msg[7] == 2) {
                        	errorPositionMsg_1("请检查气压传感器部分是否插上了跳线帽");
                        }
                        //模式错误
                        else if (msg[7] == 3) {
                        	errorPositionMsg_1("请将换档开关调节到1档位，然后重启设备");
                        }
                        //代表指令非法
                        else if (msg[7] == 255) {
                        }

                    } else {
                    	console.log("消息发送者：【 "+event.from_userid + " 】  @@ 接收消息内容 : 【  " + event.message.toString() + " 】");
                       /* console.log(event.from_userid + ": " + event.message.toString());*/
                    }
                } else {
                	console.log("消息发送者：【 "+event.from_userid + " 】  && 接收消息内容 : 【  " + event.message.toString() + " 】");
                   /* console.log(event.from_userid + ": "
                            + event.message.toString());*/
                }
            } else if (event.topic === 'sys') {
                if (event.from_userid === 'kick') {
                    // '您的账号在其他地方登录！';
                    console.log('您的账号在其他地方登录！');
                    alert('您的账号在其他地方登录！');
                    localStorage.setItem("im_obj","true");
                } else if (event.from_userid === 'to_self') {
                    console.log('不要给自己发消息！');
                }
            } else if (event.topic === 'status') {
                if (event.status === 'online') {
                    devState = 1;
                    console.log('init devState online');
                    $("#devIdStatus").text("在线");
                } else {
                    devState = 0;
                    console.log('init devState offline');
                    $("#devIdStatus").text("离线");
                }
                im.etStateSubscribe({
                    userid: devUid
                });
            } else if (event.topic === 'friend@online') {
                devState = 1;
                console.log('devState friend online');
                $("#devIdStatus").text("在线");
            } else if (event.topic === 'friend@offline') {
                devState = 0;
                console.log('devState friend offline');
                $("#devIdStatus").text("离线");
            } else if (event.topic.toString().indexOf("&TX") != -1) {
            	 console.log('receive 透传板发送的 msg. message=' + event.message);
                console.log('receive msg. topic=' + event.topic);
                displayEMsg(event.topic, event.message);
            }
        } else if (event.type === 'connect') {
            //登录成功
            console.log("登录IM成功");
            $("#devIdStatus").text("正在获取...");
            im.etGetUserStatus({
                userid: devUid,
                message: devUid
            }, function (error) {
                console.log('获取好友(设备)状态.');
                if (error) {
                    console.log("获取好友(设备)状态失败，请重试!");
                    alert('获取好友(设备)状态失败，请重试!');
                    wx.closeWindow();
                }
            });
            // opDivDisplay(true);
            reConnectCount=0;
        } else if (event.type === 'reconnect') {
            console.log('重新登录');
        } else if (event.type === 'offline') {
            console.log('已下线');
        } else if (event.type === 'close') {
            console.log('连接关闭,注销成功!');
            console.log("im_obj值： "+localStorage.getItem("im_obj"));
            if(localStorage.getItem("im_obj")!='true'){
                localStorage.removeItem("im_obj");
                im.etDisconnect();//防止重复登录问题
                reConnectCount++;
                window.setTimeout(function(){
           		 im.etConnect(opt);
                }, '3000');
            	if(reConnectCount>maxConnectCount){
                 	alert("亲，您掉线了哦，请退出我的设备的指令控制界面，重新进入");
                 	 wx.closeWindow();
                 }
            	
	            }else{
	                localStorage.removeItem("im_obj");
	            	wx.closeWindow();
	            }
        } else if (event.type === 'error') {
            console.log("用户名或密码错误! " + event.error.message + '(' + event.error.code + ')');
            alert(event.error.message + '(' + event.error.code + ')');
            wx.closeWindow();
        }
    }


    //初始化et_client,参数为一个配置选项和一个回调函数
    im = new et_cloud({
        imCallBack: onClientEvent,
    });
    im.etConnect(opt);

    function innerToString(value) {
        value = parseInt(value);
        if (value >= 10) {
            return value.toString();
        } else {
            return "0" + value;
        }
    }
    function logout() {
        im.etDisconnect();
    }

    function etPackget(packget) {
        var bcc;
        var len = packget.length;
        for (var i = 2; i < len; i++) {
            bcc ^= packget[i]
        }
        var arr = new Array(len);
        for (var i = 0; i < len; i++) {
            arr[i] = packget[i]
        }
        arr.push(bcc);
        return new buffer.Buffer(arr)
    }


    //关开灯
    function lightControl( R, G, B) {
        var obj1 = {
            userid: devUid,
            message: etPackget([0xff, 0xff, 0x00, 0x07, 0x70, 0x01, 0x20,R, G, B]),
            qos: 1
        };
        im.etChatTo(
                        obj1,
                        function (error) {
                            // changeWindow();
                            // rollBottom();
                            if (error) {
                                // if (document.getElementById(ico_fail_id)) {
                                //     document.getElementById(ico_fail_id).style.display = 'block';
                                // }
                            } else {
                                // if (document.getElementById(ico_loading_id)) {
                                //     document.getElementById(ico_loading_id).style.display = 'none';
                                // }
                            }
                            // chat_textarea.value = '';
                        })
    }


    // 根据RGB颜色值去开灯
    function openLight(R, G, B) {
        lightControl( R, G, B);
    }
    // 关灯
    function closeLight() {
        lightControl( 0x00, 0x00, 0x00);
    }


// setTimeout(function(){
//     lightControl( 0x41, 0x11, 0xff);
//     alert('ok');
// },1000);


    //存储重连次数
    var reConnectCount=0;
    //最大重连次数
    var maxConnectCount=3;
    