package com.amazonaws.lambda.demo;

import com.amazonaws.services.iotdata.AWSIotData;
import com.amazonaws.services.iotdata.AWSIotDataClientBuilder;
import com.amazonaws.services.iotdata.model.GetThingShadowRequest;
import com.amazonaws.services.lambda.runtime.Context;
import com.amazonaws.services.lambda.runtime.RequestHandler;

public class IoTSmartWindow_GetDeviceHandler implements RequestHandler<Event, String> {

    @Override
    public String handleRequest(Event event, Context context) {
        AWSIotData iotData = AWSIotDataClientBuilder.standard().build();

        GetThingShadowRequest getThingShadowRequest  = 
        new GetThingShadowRequest()
            .withThingName(event.device);

        iotData.getThingShadow(getThingShadowRequest);

        return new String(iotData.getThingShadow(getThingShadowRequest).getPayload().array());
    }
}

class Event {
    public String device;
}