package com.amazonaws.lambda.demo;

import java.util.List;
import com.amazonaws.services.iot.AWSIot;
import com.amazonaws.services.iot.AWSIotClientBuilder;
import com.amazonaws.services.iot.model.ListThingsRequest;
import com.amazonaws.services.iot.model.ListThingsResult;
import com.amazonaws.services.iot.model.ThingAttribute;
import com.amazonaws.services.lambda.runtime.Context;
import com.amazonaws.services.lambda.runtime.RequestHandler;

public class IoTSmartWindow_ListingDeviceHandler implements RequestHandler<Object, String> {

    @Override
    public String handleRequest(Object input, Context context) {

        // AWSIot 객체를 얻는다. 
        AWSIot iot = AWSIotClientBuilder.standard().build();

        // ListThingsRequest 객체 설정. 
        ListThingsRequest listThingsRequest = new ListThingsRequest();

        // listThings 메소드 호출하여 결과 얻음. 
        ListThingsResult result = iot.listThings(listThingsRequest);

        // result 객체로부터 API 응답모델 문자열 생성하여 반
        return getResponse(result);
    }

    private String getResponse(ListThingsResult result) {
        List<ThingAttribute> things = result.getThings();

        String response = "{ \"things\": [";
        for (int i =0; i<things.size(); i++) {
            if (i!=0) 
                response +=",";
            response += String.format("{\"thingName\":\"%s\", \"thingArn\":\"%s\"}", 
                                                things.get(i).getThingName(),
                                                things.get(i).getThingArn());

        }
        response += "]}";
        return response;
    }
}