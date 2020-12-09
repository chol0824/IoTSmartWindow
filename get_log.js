function Get_Logs() {
    var start_date = document.getElementById('start_date').value;
    var start_time = document.getElementById('start_time').value;
    var end_date = document.getElementById('end_date').value;
    var end_time = document.getElementById('end_time').value;
    var date_time = "/log?from=" + start_date + " " + start_time +":00&to="+ end_date + " "+ end_time + ":00";
    invokeGetLogAPI(date_time);
}

var invokeGetLogAPI = function(date_time) {
    // 디바이스 조회 URI
    // production 스테이지 편집기의 맨 위에 있는 "호출 URL/devices/IoTSmartWindowData"로 대체해야 함
    var API_URI = 'https://ymeqxvqmfg.execute-api.ap-northeast-2.amazonaws.com/production/devices/IoTSmartWindowData' + date_time;                 
    $.ajax(API_URI, {
        method: 'GET',
        contentType: "application/json",

        success: function (data, status, xhr) {
                var result = JSON.parse(data);
                printDeviceLog(result);  // 성공시, 데이터 출력을 위한 함수 호출
                console.log("data="+data);
        },
        error: function(xhr,status,e){
                alert("error");
        }
    });
};


// 데이터 출력을 위한 함수
function printDeviceLog(json){
    $('#device_logs').empty();      // id가 device_logs인 태그 안의 기존 데이터 모두 삭제

    var tr1 = document.createElement("tr");
    tr1.setAttribute("style","background-color: orange");
    var th1 = document.createElement("th");
    var th2 = document.createElement("th");
    var th3 = document.createElement("th");
    var th4 = document.createElement("th");
    var th5 = document.createElement("th");
    var th6 = document.createElement("th");
    th1.innerText = "날짜 시간";
    th2.innerText = "창문 제어";
    th3.innerText = "창문 상태";
    th4.innerText = "미세먼지";
    th5.innerText = "온도";
    th6.innerText = "습도";
    tr1.append(th1);
    tr1.append(th2);
    tr1.append(th3);
    tr1.append(th4);
    tr1.append(th5);
    tr1.append(th6);
    $('#device_logs').append(tr1);   // id가 device_state인 태그에 테이블 제목 행을 추가
        
    // 입력받은 기간의 로그값 테이블에 출력    
    for(idx in json.data){
        var tr2 = document.createElement("tr");
        var td1 = document.createElement("td");
        var td2 = document.createElement("td");
        var td3 = document.createElement("td");
        var td4 = document.createElement("td");
        var td5 = document.createElement("td");
        var td6 = document.createElement("td");
        td1.innerText = json.data[idx].timestamp;      // 날짜 & 시간 출력
        td2.innerText = json.data[idx].ControlState;   // 창문제어
        td3.innerText = json.data[idx].WindowState;    // 창문 상태
        td4.innerText = json.data[idx].Dust;           // 미세먼지 농도값
        td5.innerText = json.data[idx].Temperature;    // 온도값
        td6.innerText = json.data[idx].Humidity;       // 습도값
        tr2.appendChild(td1);
        tr2.appendChild(td2);
        tr2.appendChild(td3);
        tr2.appendChild(td4);
        tr2.appendChild(td5);
        tr2.appendChild(td6);
        $('#device_logs').append(tr2); // id가 device_logs인 태그에 테이블 행을 추가
    }
}