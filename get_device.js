function Get_Device_State() {
    invokeGetDeviceAPI();
}

function invokeGetDeviceAPI() {
    // 디바이스 조회 URI
    // production 스테이지 편집기의 맨 위에 있는 "호출 URL/devices/IoTSmartWindow"로 대체해야 함
    var API_URI = 'https://ymeqxvqmfg.execute-api.ap-northeast-2.amazonaws.com/production/devices/IoTSmartWindow';                 
    $.ajax(API_URI, {
        method: 'GET',
        contentType: "application/json",

        success: function (data, status, xhr) {
                var result = JSON.parse(data);
                printDeviceState(result);  // 성공시, 데이터 출력을 위한 함수 호출

                console.log("data="+data);
        },
        error: function(xhr,status,e){
                alert("error");
        }
    });
};

// 데이터 출력을 위한 함수
function printDeviceState(data){
    $('#device_state').empty();

    var tr1 = document.createElement("tr");
    tr1.setAttribute("style","background-color: orange");
    var th1 = document.createElement("th");
    var th2 = document.createElement("th");
    var th3 = document.createElement("th");
    var th4 = document.createElement("th");
    var th5 = document.createElement("th");
    th1.innerText = "창문 제어";
    th2.innerText = "창문 상태";
    th3.innerText = "미세먼지";
    th4.innerText = "온도";
    th5.innerText = "습도";
    tr1.append(th1);
    tr1.append(th2);
    tr1.append(th3);
    tr1.append(th4);
    tr1.append(th5);
    $('#device_state').append(tr1);   // id가 device_state인 태그에 테이블 제목 행을 추가
        

    var tr2 = document.createElement("tr");
    var td1 = document.createElement("td");
    var td2 = document.createElement("td");
    var td3 = document.createElement("td");
    var td4 = document.createElement("td");
    var td5 = document.createElement("td");
    td1.innerText = data.state.reported.ControlState;
    td2.innerText = data.state.reported.WindowState;
    td3.innerText = data.state.reported.Dust;
    td4.innerText = data.state.reported.Temperature;
    td5.innerText = data.state.reported.Humidity;
    tr2.appendChild(td1);
    tr2.appendChild(td2);
    tr2.appendChild(td3);
    tr2.appendChild(td4);
    tr2.appendChild(td5);
    $('#device_state').append(tr2); // id가 devices인 태그에 테이블 행을 추가
}