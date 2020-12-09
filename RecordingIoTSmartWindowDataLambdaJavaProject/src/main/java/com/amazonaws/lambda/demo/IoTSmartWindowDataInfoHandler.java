package com.amazonaws.lambda.demo;

import java.text.SimpleDateFormat;
import java.util.TimeZone;
import com.amazonaws.services.dynamodbv2.AmazonDynamoDB;
import com.amazonaws.services.dynamodbv2.AmazonDynamoDBClientBuilder;
import com.amazonaws.services.dynamodbv2.document.DynamoDB;
import com.amazonaws.services.dynamodbv2.document.Item;
import com.amazonaws.services.dynamodbv2.document.PutItemOutcome;
import com.amazonaws.services.dynamodbv2.document.spec.PutItemSpec;
import com.amazonaws.services.dynamodbv2.model.ConditionalCheckFailedException;
import com.amazonaws.services.lambda.runtime.Context;
import com.amazonaws.services.lambda.runtime.RequestHandler;

public class IoTSmartWindowDataInfoHandler implements RequestHandler<Thing, String> {
    private DynamoDB dynamoDb;
    private String DYNAMODB_TABLE_NAME = "IoTSmartWindowData";

    @Override
    public String handleRequest(Thing input, Context context) {
        this.initDynamoDbClient();
        context.getLogger().log("Input: " + input);
        
        persistData(input);
        return "Success in storing to DB!";
    }

    // DynamoDB에 IoTSmartWindow 데이터 저장하는 함수(창문제어 상태, 창문 상태, 온도값, 습도값, 미세먼지 농도값 데이터 저장)
    private PutItemOutcome persistData(Thing thing) throws ConditionalCheckFailedException {

        // Epoch Conversion Code: https://www.epochconverter.com/
        SimpleDateFormat sdf = new SimpleDateFormat ( "yyyy-MM-dd HH:mm:ss");			// 시간 format
        sdf.setTimeZone(TimeZone.getTimeZone("Asia/Seoul"));							// 서울 지역
        String timeString = sdf.format(new java.util.Date (thing.timestamp*1000));		// 타임스탬프

        return this.dynamoDb.getTable(DYNAMODB_TABLE_NAME)
                .putItem(new PutItemSpec().withItem(new Item().withPrimaryKey("time", thing.timestamp)	// Primary Key
                		.withString("ControlState", thing.state.reported.ControlState)	// 창문제어 상태값 추가
                		.withString("WindowState", thing.state.reported.WindowState)	// 창문 상태값 추가
                        .withString("Temperature", thing.state.reported.Temperature)	// 온도값 추가
                        .withString("Humidity", thing.state.reported.Humidity)			// 습도값 추가
                        .withString("Dust", thing.state.reported.Dust)					// 미세먼지 농도값 추가
                        .withString("timestamp",timeString)));							// 타임스탬프값 추가
    }

    private void initDynamoDbClient() {
        AmazonDynamoDB client = AmazonDynamoDBClientBuilder.standard().withRegion("ap-northeast-2").build();

        this.dynamoDb = new DynamoDB(client);
    }

}

class Thing {
    public State state = new State();
    public long timestamp;					// 타임스탬프(기본 키값: 항목의 고유한 식별값)

    public class State {
        public Tag reported = new Tag();
        public Tag desired = new Tag();

        public class Tag {
        	public String ControlState;		// 창문제어 상태(AUTO-자동/MANUAL-수동)
        	public String WindowState;		// 창문 상태(OPEN-열림/CLOSE-닫힘)
            public String Temperature;		// 온도
            public String Humidity;			// 습도
            public String Dust;				// 미세먼지 농도
        }
    }
}