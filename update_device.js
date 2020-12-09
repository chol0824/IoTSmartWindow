// 창문 자동 제어
function Auto_Control() {
    // JSON 메세지(자동 제어)
    var auto_json = {tags:[{tagName:"ControlState", tagValue:"AUTO"},{tagName:"WindowState", tagValue:"CLOSE"}]}
    var Auto_json = JSON.stringify(auto_json);
    document.getElementById("current_control_state").innerHTML="현재 우리집 창문은 자동 제어 중입니다 :)";
    invokeUpdateDeviceAPI(Auto_json);
}


// 창문 수동 열기
function Manual_Open() {
    // JSON 메세지(수동 창문 열기)
    var manual_open_json = {tags:[{tagName:"ControlState", tagValue:"MANUAL"},{tagName:"WindowState", tagValue:"OPEN"}]}
    var Manual_Open_json = JSON.stringify(manual_open_json);
    document.getElementById("current_control_state").innerHTML="현재 우리집 창문은 수동 제어 중입니다 :)";
    invokeUpdateDeviceAPI(Manual_Open_json);
}


// 창문 수동 닫기
function Manual_Close() {
    // JSON 메세지(수동 창문 닫기)
    var manual_close_json = {tags:[{tagName:"ControlState", tagValue:"MANUAL"},{tagName:"WindowState", tagValue:"CLOSE"}]}
    var Manual_Close_json = JSON.stringify(manual_close_json);
    document.getElementById("current_control_state").innerHTML="현재 우리집 창문은 수동 제어 중입니다 :)";
    invokeUpdateDeviceAPI(Manual_Close_json);
}


var invokeUpdateDeviceAPI = function(control_json) {
    //  URI
    // production 스테이지 편집기의 맨 위에 있는 "호출 URL/devices/IoTSmartWindow"로 대체해야 함
    var API_URI = 'https://ymeqxvqmfg.execute-api.ap-northeast-2.amazonaws.com/production/devices/IoTSmartWindow';                 
    $.ajax({
        url: API_URI,
        method: 'PUT',
        contentType: "application/json",
        dataType="json",
        data = control_json,
        async = true,
        success: function (data, status, xhr) {
                var result = JSON.parse(data);
                console.log("data="+data);
        },
        error: function(xhr,status,e){
                alert("error");
        }
});