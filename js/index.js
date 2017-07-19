mui.init();

//自定义$
//function $(name) {
//  var obj;
//  if (name.match(/^#/)) {
//     obj = document.getElementById(name.substr(1));
//  } else if (name.match(/^\./)) {
//      obj = document.getElementsByClassName(name.substr(1));
//  } else {
//      obj = document.getElementsByTagName(name);
//  }
//
//  return obj;
//}

var currMonth = new Date().getMonth() + 1; //获取月份判断季节
var statusBarBgColors = ['#76A4A1', '#91A5AC', '#819BAD', '#919799'];  //各季节壁纸对应状态栏颜色

if (2 <= currMonth && currMonth <= 4) {
	$('.weather-cover').eq(0).attr("class",'weather-cover-spring');
} else if (5 <= currMonth && currMonth <= 7) {
	$('.weather-cover').eq(0).attr("class", 'weather-cover-summer');	
} else if (8 <= currMonth && currMonth <= 10) {
	$('.weather-cover').eq(0).attr("class",'weather-cover-autumn');
} else {
	$('.weather-cover').eq(0).attr("class",'weather-cover-winter');
}	

//先获取ip地址，再根据ip地址获取天气信息
mui.ajax({
	type:	'get',
	async:  'false',
	
	url: 'http://api.k780.com:88/?app=ip.local&appkey=25209&sign=c07e6049897f22790d62d9aac21c1636&format=json',
	dataType: 'json',
	jsonp: 'callback',
	jsonpCallback: 'data',
	
	success: function (data) {
		var data = data;
		var weaid = '';
		
		if (data.success != '1') {
			console.log('获取信息失败');
			
		} else {
			weaid = data.result.ip;
		}
		
		mui.ajax({
			type:	'get',
			async:  'false',
			
			url: 'http://api.k780.com:88/?app=weather.today&weaid='+weaid+'&appkey=25209&sign=c07e6049897f22790d62d9aac21c1636&format=json',
			dataType: 'json',
			jsonp: 'callback',
			jsonpCallback: 'data',
			
			success: function (data) {
				var weatherInfo = data.result;
				
				$('#weather-citynm').innerHTML = (weatherInfo.citynm);
				
				$('#weatherInfo-temperature_curr').innerHTML = (parseInt(weatherInfo.temperature_curr));
				
				$('#weatherInfo-winp').innerHTML = (weatherInfo.winp);
				
				$('.weatherInfo-wind')[0].innerHTML = (weatherInfo.wind);
				$('.weatherInfo-wind')[1].innerHTML = (weatherInfo.wind);
				
				$('#weatherInfo-weather').innerHTML = (weatherInfo.weather);
				
				$('.weatherInfo-humidity')[0].innerHTML = (weatherInfo.humidity);
				$('.weatherInfo-humidity')[1].innerHTML = (weatherInfo.humidity);
				
				$('#weatherInfo-temp_high').innerHTML = (weatherInfo.temp_high);
				
				$('#weatherInfo-temp_low').innerHTML = (weatherInfo.temp_low);	
			}
		});
	}
});

   		