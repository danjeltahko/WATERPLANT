import json
import requests

TELEGRAM_MSG = "time to water plant"

def lambda_handler(event, context):
    bot_request = f'https://api.telegram.org/bot{TELEGRAM_TOKEN}/sendMessage?chat_id={TELEGRAM_CHAT_ID}&text={TELEGRAM_MSG}'
    print(event)
    print(bot_request)
    response = requests.get(bot_request) 
    return {
       'statusCode': response.status_code,
       'msg': json.dumps('Message sent with success!'),
       'body': json.dumps(response.json())
   }