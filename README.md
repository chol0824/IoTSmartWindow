# IoTSmartWindow
IoT 클라우드 플랫폼 - 기말 과제



IoTSmartWindow 미세먼지 농도와 실내 온도에 따라 창문 열기/닫기를 자동으로 제어합니다. 외출하거나 화상강의 등으로 인해 직접 창문을 닫기 힘든 상황일 때 웹 애플리케이션을 통해 수동제어로 창문 열기/닫기가 가능합니다. 집 실외에 미세먼지 측정 센서가 부착되어있고, 실내에는 온습도 센서가 부착되어 있어서 실시간으로 미세먼지 농도와 온습도를 측정하여 AWS DynamoDB에 저장합니다. 웹 애플리케이션에서는 현재 우리 집의 상태 조회(창문 제어상태(자동/수동), 창문 상태(열림/닫힘), 온도, 습도, 미세먼지 농도)가 가능하고, 특정 기간을 입력하면 해당 기간의 우리집 상태 로그를 확인할 수 있으며, 창문 자동제어, 수동제어 버튼을 눌러서 창문 상태(열림/닫힘)을 제어할 수 있습니다.



IoT 디바이스: Arduino MKR WiFi 1010

사용한 센서: 온습도 센서, 미세먼지 측정 센서, 서보모터(창문 열기/닫기 제어), 3색LED(미세먼지 상태 표시)



<창문 자동 제어>
  - 실내 적정 온도: 겨울 18~20, 여름 24~26 → IoTSmartWindow에서는 18~26도를 적정 온도로 지정
  - 미세먼지 농도: 좋음 0~30, 보통 31~80, 나쁨 81~
  
  → 미세먼지 농도에 따라 3색LED 색 변경 - 좋음: 초록, 보통: 파랑, 나쁨: 빨강
  → 미세먼지 농도가 좋음~보통이고 적정 온도(18~26)이면 창문을 자동으로 열어 환기시킵니다.
  → 미세먼지 농도가 나쁨이거나 적정 온도가 아니라면(너무 춥거나 더우면) 창문을 자동으로 닫습니다.



<IoT 백엔드 - AWS 서비스>
  AWS Lambda
    - 자바 코드를 AWS로 업로드 할 수 있음
    - 실행시킬 때 JSON 형식 사용

  AMazon DynamoDB
    - (키-값) 문서 저장
    - IoTSmartWindow에서 측정한 센서값, 창문제어 상태, 창문 상태 데이터 저장

  API GateWay - API 관리함
    - REST API: 클라이언트 - 서버 통신 지원
      - 리소스(=URI)
        - 서버는 고유한 ID를 가지는 리소스 가짐
        - 클라이언트는 리소스에 요청 보냄
        - 보통 소문자로 명사 씀
        - document 이름은 단수명사, 컬렉션 이름은 복수명사 사용

      - HTTP 메소드
        - 서버에 요청 보내는 방식(CRUD 연산)
        - GET	: 조회(해당되는 리소스 요청)
        - POST	: 생성
        - PUT	: 변경
        - DELETE: 삭제

      - 자원의 표현
        - 클라이언트와 서버가 데이터를 주고받는 형태(ex. json)



<프로젝트 직접 해보기>
1. 디바이스 등록(디바이스 이름: IoTSmartWindow)
	- 아두이노 X.509 인증서 발급: AWS IoT에서 디바이스 인증하는 데 사용
		1) 아두이노 예제 - ArduinoECCX08 - Tools - ECCX08CSR 파일 열고 아두이노 연결해서 실행시킴
		2) 시리얼 모니터에서 빈칸 입력, Common Name에 IoTSmartWindow 입력, 0 입력, Y 입력
		3) 인증서 내용 메모장에 복붙하고 "csr.txt"로 저장

	- AWS IoT에 디바이스 등록 및 CSR 등록
		1) AWS IoT - 사물 - 단일 사물 생성, IoTSmartWindow 입력
		2) CSR을 통한 생성 - 인증서 등록

	- AWS IoT에 정책 생성 및 인증서에 등록: AWS IoT 작업 수행할 수 있는 권한 부여
		1) AWS IoT - 보안 - 정책 - 정책 생성
		2) 이름	AllowEveryIoTThing
		    작업	iot:*
		    리소스 ARN *
		    효과	허용
		3) AWS IoT - 보안 - 인증서 - 방금 만든 인증서 선택
		    작업 - 정책 연결 - AllowEveryIoTTHing - 연결
		4) AWS IoT - 보안 - 인증서 - 방금 만든 인증서
		    작업 - 다운로드
		5) AWS IoT - 설정 - 엔드포인트 복사
		6) 아두이노 예제 - Arduino Cloud Provider Examples - AWS IoT - AWS_IoT_WiFi 파일 열기
		    arduino_secrets.h에서 와이파이 SSID, Password 설정
		    SECRET_BROKER에 엔드포인트 붙여넣기
		    SECRET_CERTIFICATE[]에 "~perm.crt" 다운받은 인증서 내용 복붙 후 코드 실행
  - 아두이노 실행
    1) arduino_secrets.h에서 와이파이 SSID, Password 설정
		    SECRET_BROKER에 엔드포인트 붙여넣기
		    SECRET_CERTIFICATE[]에 "~perm.crt" 다운받은 인증서 내용 복붙 후 코드 실행 
    2) Smart_Window.ino 코드보고 아두이노 보드 구성
    3) Smart_Window.ino 코드 아두이노 보드로 업로드
    4) 시리얼 모니터로 와이파이, MQTT 연결 확인, 센서 측정값 확인


2. AWS 자격증명: 자바에서 AWS Lambda를 사용하기 위한 인증(사용자이름: SmartWindow)
	- 자격증명 설정
		1) IAM - 사용자 - 사용자 추가
			사용자이름: SmartWindow
			액세스 유형: 프로그래밍 방식 액세스
		2) 다음 - 기존 정책 직접 연결 - AdministratorAccess 선택 (모든 권한 가짐) - 다음 - 다음 - 사용자 만들기 - 닫기
		3) SmartWindow(방금 만든거) - 보안 자격 증명 - 액세스 키 만들기 - .CSV 파일 만들기
		4) 이클립스 - window - preferences - AWS Toolkit
			Acees Key ID: 복붙(방금 다운받은 csv 파일 참고)
			Secret Access Key: 복붙(방금 다운받은 csv 파일 참고)
			apply and close - yes
      

3. IAM role 만들기(IAM role이름: lambda_basic_execution_role_iot_access)
	- 역할 만들기
		1) IAM - 역할 - 역할 만들기
			일반 사용 사례: Lambda 클릭 - 다음
			정책 이름: AWSLambdaBasicExecutionRole 선택 - 다음 - 다음
			역할 이름: lambda_basic_execution_role_iot_access 입력 - 역할 만들기
    2) IAM - 역할 - 역할 만들기
      일반 사용 사례: Lambda 클릭 - 다음
      정책 이름: AmazonDynamoDBFullAccess 선택 - 다음 - 다음
      역할 이름: lambda_basic_execution_with_DynamoDB


4. DynamoDB(DynamoDB에 아두이노 센서값, 창문제어 상태, 창문 상태 저장)
	- 테이블 만들기
		1) DynamoDB - 테이블 만들기
			테이블 이름: IoTSmartWindowData
			기본 키: time	번호
			생성


5.  AWS Lambda 함수(DynamoDB에 아두이노 센서값 저장하는 람다 함수)
	- Lambda 함수 만들기
		1) 이클립스 - 주황색 박스 아이콘 클릭 - New AWS Lambda Java Project
			Project name: RecordingIoTSmartWindowDataLambdaJavaProject
			Class Name: IoTSmartWindowDataInfoHandler
			Input Type: Custom
		2) 코드 변경
	
	- Lambda 함수 업로드
		1) 우클릭 - AWS Lambda - Upload function to AWS Lambda
			Select the AWS Region: Asia Pacific (Seoul)
			Create a new Lambda function: IoTSmartWindowDataInfoFunction
			다음
			IAM Role: lambda_basic_execution_with_DynamoDB
			S3 Bucket: iotsmartwindow 
			Finish

	- Lambda 함수 실행(임의로 테스트)
		1) 우클릭 - AWS Lambda - Run function on AWS Lambda
			Enter the JSON input for your function:
				{
					"state":{
						"reported": {
							"ControlState":"AUTO", "WindowState":"OPEN", "Temperature":"25.00", "Humidity":"15.00", "Dust":"0.12"
						}
					},
					"timestamp": 1604467313
				}
		2) DynamoDB - IoTSmartWindowData - 항목에서 확인

		*참고) Cloud Watch - 로그 그룹 - 람다함수 선택 - 로그 스트림 클릭하면 자바에서 AWS로 보낸 메시지 확인 가능


6. AWS IoT Rule
	- 규칙 만들기
		1) AWS IoT - 동작 - 규칙 - 생성
			이름: DataUploadingRule
			규칙 쿼리 설명문: SELECT * FROM '$aws/things/IoTSmartWindow/shadow/update/accepted'
		2) 하나 이상의 작업을 설정 - 작업 추가 - 메시지 데이터를 전달하는 Lambda 함수 호출 - 작업 구성 - IoTSmartWindowDataInfoFunction - 작업추가 - 규칙생성
		3) 아두이노 실행시키고 시리얼 모니터로 메시지 보내지고 있는지 확인
		4) DynamoDB - IoTSmartWindowData - 항목에서 확인
    

7. 디바이스 목록조회 REST API
	- Lambda 함수 만들기
		1) 이클립스 - 주황색 박스 아이콘 클릭 - New AWS Lambda Java Project
			Project name: IoTSmartWindow_ListingDevice_LambdaJavaProject
			Class Name: IoTSmartWindow_ListingDeviceHandler
			Input Type: Custom
		2) 프로젝트 하위에 있는 pom.xml 파일맨 아래 </dependencies> 바로 위에 코드 추가하고 저장 필수!
			<dependency>
      			    <groupId>com.amazonaws</groupId>
      			    <artifactId>aws-java-sdk-iot</artifactId>
    			</dependency>
		3) 코드 변경
	
	- Lambda 함수 업로드
		1) 우클릭 - AWS Lambda - Upload function to AWS Lambda
			Select the AWS Region: Asia Pacific (Seoul)
			Create a new Lambda function: IoTSmartWindowListingDeviceFunction
			다음
			IAM Role: lambda_basic_execution_iot_access (AWS IoT에 대한 모든 접근 권한 허용)
			S3 Bucket: iotsmartwindow 
			Finish

	- Lambda 함수 실행(임의로 테스트)
		1) 우클릭 - AWS Lambda - Run function on AWS Lambda
			Enter the JSON input for your function: 아무것도 입력 안 함
		2) Console창에 디바이스 목록 뜨는지 확인


8. 디바이스 상세조회 REST API
	- Lambda 함수 만들기
		1) 이클립스 - 주황색 박스 아이콘 클릭 - New AWS Lambda Java Project
			Project name: IoTSmartWindow_GetDevice_LambdaJavaProject
			Class Name: IoTSmartWindow_GetDeviceHandler
			Input Type: Custom
		2) 프로젝트 하위에 있는 pom.xml 파일맨 아래 </dependencies> 바로 위에 코드 추가하고 저장 필수!
			<dependency>
      			    <groupId>com.amazonaws</groupId>
      			    <artifactId>aws-java-sdk-iot</artifactId>
    			</dependency>
		3) 코드 변경
	
	- Lambda 함수 업로드
		1) 우클릭 - AWS Lambda - Upload function to AWS Lambda
			Select the AWS Region: Asia Pacific (Seoul)
			Create a new Lambda function: IoTSmartWindowGetDeviceFunction
			다음
			IAM Role: lambda_basic_execution_iot_access (AWS IoT에 대한 모든 접근 권한 허용)
			S3 Bucket: iotsmartwindow 
			Finish

	- Lambda 함수 실행(임의로 테스트)
		1) 우클릭 - AWS Lambda - Run function on AWS Lambda
			Enter the JSON input for your function: 
			{
			    "device" : "IoTSmartWindow"
			}
		2) Console창에 디바이스 목록 뜨는지 확인


9. 디바이스 상태변경 REST API
	- Lambda 함수 만들기
		1) 이클립스 - 주황색 박스 아이콘 클릭 - New AWS Lambda Java Project
			Project name: IoTSmartWindow_UpdatingDevice_LambdaJavaProject
			Class Name: IoTSmartWindow_UpdatingDeviceHandler
			Input Type: Custom
		2) 프로젝트 하위에 있는 pom.xml 파일맨 아래 </dependencies> 바로 위에 코드 추가하고 저장 필수!
			<dependency>
      			    <groupId>com.amazonaws</groupId>
      			    <artifactId>aws-java-sdk-iot</artifactId>
    			</dependency>
		3) 코드 변경
	
	- Lambda 함수 업로드
		1) 우클릭 - AWS Lambda - Upload function to AWS Lambda
			Select the AWS Region: Asia Pacific (Seoul)
			Create a new Lambda function: IoTSmartWindowUpdatingDeviceFunction
			다음
			IAM Role: lambda_basic_execution_iot_access (AWS IoT에 대한 모든 접근 권한 허용)
			S3 Bucket: iotsmartwindow 
			Finish

	- Lambda 함수 실행(임의로 테스트)
		1) 우클릭 - AWS Lambda - Run function on AWS Lambda
			Enter the JSON input for your function:
			{
 			   "device": "IoTSmartWindow",
 			   "tags" : [
 			       {
 			           "tagName": "ControlState",
 			           "tagValue": "MANUAL"
 			       },
 			       {
  			          "tagName": "WindowState",
  			          "tagValue": "OPEN"
  			      }   
   			   ]
			}
		2) Console창에 디바이스 목록 뜨는지 확인


10. 디바이스 로그조회 REST API
	- Lambda 함수 만들기
		1) 이클립스 - 주황색 박스 아이콘 클릭 - New AWS Lambda Java Project
			Project name: IoTSmartWindow_GetLog_LambdaJavaProject
			Class Name: IoTSmartWindow_GetLogHandler
			Input Type: Custom
		2) 프로젝트 하위에 있는 pom.xml 파일맨 아래 </dependencies> 바로 위에 코드 추가하고 저장 필수!
			<dependency>
      			    <groupId>com.amazonaws</groupId>
      			    <artifactId>aws-java-sdk-iot</artifactId>
    			</dependency>
		3) 코드 변경
	
	- Lambda 함수 업로드
		1) 우클릭 - AWS Lambda - Upload function to AWS Lambda
			Select the AWS Region: Asia Pacific (Seoul)
			Create a new Lambda function: IoTSmartWindowGetLogFunction
			다음
			IAM Role: lambda_basic_execution_with_DynamoDB
			S3 Bucket: iotsmartwindow 
			Finish

	- Lambda 함수 실행(임의로 테스트)
		1) 우클릭 - AWS Lambda - Run function on AWS Lambda
			Enter the JSON input for your function:
			{ "device": "IoTSmartWindowData", "from":"2020-12-07 00:00:00", "to": "2020-12-07 18:00:00"}  
		2) Console창에 디바이스 목록 뜨는지 확인


11. API Gateway 만들기
	- API Gateway 만들기	
		1) API Gateway - API 생성 - REST API 구축
			API 이름: IoTSmartWindow_API
			API 생성
	- 리소스 생성
		1) 작업 - 리소스 생성							→ 모든 디바이스
			리소스 이름: devices
			리소스 생성
		2) /devices 클릭 - 작업 - 메소드 생성 - GET					→ 모든 디바이스 목록 조회
			Lambda 함수: IoTSmartWindowListingDeviceFunction
			저장 - 확인
		3) /devices 클릭 - 작업 - 리소스 생성						→ 특정 디바이스
			리소스 이름: device
			리소스 경로: {device}
			리소스 생성
		4) /{devices} 클릭 - 작업 - 메소드 생성 - GET					→ 특정 디바이스 상세 조회
			Lambda 함수: IoTSmartWindowGetDeviceFunction
			저장 - 확인
		5) /{devices} GET 클릭 - 통합요청 - 매핑 템플릿 - 매핑 템플릿 추가
			application/json 입력 - 확인표시 클릭 - 예
			템플릿 생성 아래에
			{
			    "device": "$input.params('device')"
			}
			저장
		6) /{devices} 클릭 - 작업 - 메소드 생성 - PUT					→ 특정 디바이스 상태 변경
			Lambda 함수: IoTSmartWindowUpdatingDeviceFunction
			저장 - 확인
		7) 모델 생성
			1) API Gateway - IoTSmartWindow_API - 모델(왼쪽 메뉴)
				모델 이름: IoTSmartWindowUpdateDeviceInput
				콘텐츠 유형: application/json
				모델 스키마:
				{
 			  	    "$schema": "http://json-schema.org/draft-04/schema#",
  			  	    "title": "IoTSmartWindowUpdateDeviceInput",
  			  	    "type" : "object",
  			  	    "properties" : {
 			      	        "tags" : {
 			          	            "type": "array",
 			          	            "items": {
 			              	    "type": "object",
 			              	    "properties" : {
 			 		        "tagName" : { "type" : "string"},
 			                  	        "tagValue" : { "type" : "string"}
 			                    	    }
 			         	            }
 			     	        }
 			  	    }
				}
		8) /{devices}의 PUT 클릭 - 메소드 요청 - 요청 본문 - 모델 추가
			콘텐츠 유형: application/json
			모델 이름: IoTSmartWindowUpdateDeviceInput
		9) /{device}의 PUT 클릭 - 통합요청 - 매핑 템플릿 - 템플릿 추가 - 요청 본문
			application/json 입력 - 확인표시 클릭 - 예
			템플릿 생성 아래에
			#set($inputRoot = $input.path('$'))
			{
			    "device": "$input.params('device')",
			    "tags" : [
			    ##TODO: Update this foreach loop to reference array from input json
 			       #foreach($elem in $inputRoot.tags)
			       {
 			           "tagName" : "$elem.tagName",
 			           "tagValue" : "$elem.tagValue"
 			       } 
 			       #if($foreach.hasNext),#end
 			       #end
			   ]
			}
			저장
		10) /{device} 클릭 - 리소스 생성
			리소스 이름: log
			리소스 생성
		11) /log 클릭 - 메소드 생성 - GET						→ 특정 디바이스 로그 조회
			Lambda 함수: IoTSmartWindowGetLogFunction
			저장 - 확인
		12) /log의 GET - 메소드 요청 - URL 쿼리 문자열 파라미터
			쿼리 문자열 추가 - 이름: from - 확인
			쿼리 문자열 추가 - 이름: to - 확인

		13) /log의 GET - 통합 요청 - 매핑 템플릿 - 매핑 템플릿 추가 - 요청 본문
			application/json 입력 - 확인표시 클릭 - 예
			템플릿 생성 아래에
			{
			  "device": "$input.params('device')",
			  "from": "$input.params('from')",
			  "to":  "$input.params('to')"
			}
			저장

	- 메소드 테스트
		1) 모든 메소드마다 - 테스트
			/devices의 GET: 아무것도 입력X
			/devices/{device}의 GET: 경로에 IoTSmartWindow 입력
			/devices/{device}의 PUT: 경로에 IoTSmartWindow 입력
					         요청 본문
						{
						    "tags" : [
						            {
						                "tagName": "ControlState",
						                "tagValue": "AUTO"
						            },
						            {
						                "tagName": "WindowState",
						                "tagValue": "OPEN"
						            }   
						    ]
						}
{
    "tags" : [
        {
	"tagName": "ControlState",
	"tagValue": "AUTO"
        },
        {
            "tagName": "WindowState",
            "tagValue": "OPEN"
        }   
    ]
}

			/devices/{device}/log의 GET: 경로에 IoTSmartWindowData 입력
						쿼리 문자열
						from="2020-12-07 00:00:00"&to="2020-12-07 18:00:00"
			테스트 - 아래 응답 본문 확인하기
	- CORS 활성화
		1) 모든 리소스, 메소드마다 작업 - CORS 활성화 - CORS 활성화 및 기존의 CORS 헤더 대체 - 예

	- API 배포
		1) 작업 - API 배포
			배포 스테이지: 새 스테이지
			스테이지 이름: production
			배포
		2) URL 호출 클릭하면 웹 브라우저 볼 수 있음(URL 뒤에 아래처럼 입력)
			URL/devices			→ 디바이스 목록 조회 가능
			URL/devices/IoTSmartWindow	→ 특정 디바이스 상세 조회 가능
			URL/devices/IoTSmartWindowData/log?from=2020-12-07 00:00:00&to=2020-12-07 18:00:00


12. 웹 애플리케이션 만들기
  - HTML, js파일 만들기
      1) IoTSmartWindow.html  → 웹 페이지
      2) get_devices.js       → 디바이스 상태 조회
      3) update_device.js     → 디바이스 상태 변경
      4) get_log.js           → 디바이스 로그 조회



프로젝트에 참고한 사이트:
  
  https://kocoafab.cc/product/smarthome
  
  https://blog.naver.com/roboholic84/221086205218
