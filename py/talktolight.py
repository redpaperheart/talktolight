#!/usr/bin/env python

# Copyright (C) 2017 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


from __future__ import print_function

import argparse
import os.path
import json

import google.auth.transport.requests
import google.oauth2.credentials

from google.assistant.library import Assistant
from google.assistant.library.event import EventType
from google.assistant.library.file_helpers import existing_file

from pythonosc import osc_message_builder
from pythonosc import udp_client

from threading import Timer
from time import sleep

OSC_DESTINATION       = "127.0.0.1"

OSC_PORT              = 12345 #edit: Shuvo
OSC_QUESTION_STARTED  = "/assistant/question/start"
OSC_RESPONSE_STARTED  = "/assistant/response/start"
OSC_RESPONSE_ENDED    = "/assistant/response/end"

OSC_MODE_LIGHT        = "/mode/light"
OSC_MODE_DISCO        = "/mode/disco"
OSC_MODE_CAT          = "/mode/cat"

OSC_MODE_SPOTLIGHT    = "/mode/spotlight"
OSC_MODE_SCANNER      = "/mode/scanner"
OSC_MODE_AURORA       = "/mode/aurora"
OSC_MODE_RAINBOW      = "/mode/rainbow"
OSC_MODE_ROBOT        = "/mode/robot"
OSC_MODE_SPACESHIP    = "/mode/spaceship"
OSC_MODE_ALIEN        = "/mode/alien"
OSC_MODE_FIREFLY      = "/mode/firefly"
OSC_MODE_KITT         = "/mode/kitt"

OSC_MUSIC_PLAY        = "/music/play"
OSC_MUSIC_STOP        = "/music/stop"

DEVICE_API_URL = 'https://embeddedassistant.googleapis.com/v1alpha2'

# Create OSC client to communicate with graphics app
global osc_client
osc_client = udp_client.SimpleUDPClient(OSC_DESTINATION, OSC_PORT)

# Create OSC clients to communicate with music app and menu app
#global osc_music_client
#osc_music_client = udp_client.SimpleUDPClient(OSC_MUSIC_DESTINATION, OSC_PORT)
#global osc_menu_client
#osc_menu_client = udp_client.SimpleUDPClient(OSC_MENU_DESTINATION, OSC_PORT)

#adding send_osc function
def send_osc(message):
    print("Sending OSC message: " + str(message))
    osc_client.send_message(str(message), 0)
    #osc_music_client.send_message(str(message), 0)
    #osc_menu_client.send_message(str(message), 0)
          
def process_device_actions(event, device_id):
    if 'inputs' in event.args:
        for i in event.args['inputs']:
            if i['intent'] == 'action.devices.EXECUTE':
                for c in i['payload']['commands']:
                    for device in c['devices']:
                        if device['id'] == device_id:
                            if 'execution' in c:
                                for e in c['execution']:
                                    if 'params' in e: #e['params']:
                                        yield e['command'], e['params']
                                    else:
                                        yield e['command'], None


def process_event(event, device_id):
    """Pretty prints events.

    Prints all events that occur with two spaces between each new
    conversation and a single space between turns of a conversation.

    Args:
        event(event.Event): The current event to process.
    """
    if event.type == EventType.ON_CONVERSATION_TURN_STARTED:
        print()
    #if event.type == EventType.ON_RENDER_RESPONSE:
     #   print()

    print(event)

    if (event.type == EventType.ON_CONVERSATION_TURN_FINISHED and
            event.args and not event.args['with_follow_on_turn']):
        print()
        
    if event.type == EventType.ON_CONVERSATION_TURN_STARTED:
        send_osc(OSC_QUESTION_STARTED)

    elif event.type == EventType.ON_RESPONDING_STARTED:
        send_osc(OSC_RESPONSE_STARTED)

    elif event.type == EventType.ON_CONVERSATION_TURN_FINISHED:
        send_osc(OSC_RESPONSE_ENDED)
        
    #custom device actions
    if event.type == EventType.ON_DEVICE_ACTION:
        for command, params in process_device_actions(event, device_id):
            print('Do command', command, 'with params', str(params))
            if command == "Osc":
                number = params['number']
                #number = int( params['number'] )
                print('Attempting to send test signal')
                send_osc(number);
            if command == "catMode":
                print('Attempting change to cat mode')
                send_osc( OSC_MODE_CAT );
            if command == "lightMode":
                print('Attempting change to story mode')
                send_osc( OSC_MODE_LIGHT );
            if command == "discoMode":
                print('Attempting change to disco mode')
                send_osc( OSC_MODE_DISCO );
            if command == "spotlightMode":
                print('Attempting change to spotlight mode')
                send_osc( OSC_MODE_SPOTLIGHT );
            if command == "scannerMode":
                print('Attempting change to scanner mode')
                send_osc( OSC_MODE_SCANNER );
            if command == "rainbowMode":
                print('Attempting change to rainbow mode')
                send_osc( OSC_MODE_RAINBOW );
            if command == "auroraMode":
                print('Attempting change to aurora mode')
                send_osc( OSC_MODE_AURORA );
            if command == "robotMode":
                print('Attempting change to robot mode')
                send_osc( OSC_MODE_ROBOT );
            if command == "spaceshipMode":
                print('Attempting change to spaceship mode')
                send_osc( OSC_MODE_SPACESHIP );
            if command == "alienMode":
                print('Attempting change to alien mode')
                send_osc( OSC_MODE_ALIEN );
            if command == "fireflyMode":
                print('Attempting change to firefly mode')
                send_osc( OSC_MODE_FIREFLY );
            if command == "kittMode":
                print('Attempting change to kitt mode')
                send_osc( OSC_MODE_KITT );


def register_device(project_id, credentials, device_model_id, device_id):
    """Register the device if needed.

    Registers a new assistant device if an instance with the given id
    does not already exists for this model.

    Args:
       project_id(str): The project ID used to register device instance.
       credentials(google.oauth2.credentials.Credentials): The Google
                OAuth2 credentials of the user to associate the device
                instance with.
       device_model_id: The registered device model ID.
       device_id: The device ID of the new instance.
    """
    base_url = '/'.join([DEVICE_API_URL, 'projects', project_id, 'devices'])
    device_url = '/'.join([base_url, device_id])
    session = google.auth.transport.requests.AuthorizedSession(credentials)
    r = session.get(device_url)
    print(device_url, r.status_code)
    if r.status_code == 404:
        print('Registering....', end='', flush=True)
        r = session.post(base_url, data=json.dumps({
            'id': device_id,
            'model_id': device_model_id,
            'client_type': 'SDK_LIBRARY'
        }))
        if r.status_code != 200:
            raise Exception('failed to register device: ' + r.text)
        print('\rDevice registered.')


def main():
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('--credentials', type=existing_file,
                        metavar='OAUTH2_CREDENTIALS_FILE',
                        default=os.path.join(
                            os.path.expanduser('~/.config'),
                            'google-oauthlib-tool',
                            'credentials.json'
                        ),
                        help='Path to store and read OAuth2 credentials')
    parser.add_argument('--device_model_id', type=str,
                        metavar='DEVICE_MODEL_ID', required=True,
                        help='The device model ID registered with Google')
    parser.add_argument('--project_id', type=str,
                        metavar='PROJECT_ID', required=False,
                        help=('The project ID used to register'
                              'device instances'))

    args = parser.parse_args()
    with open(args.credentials, 'r') as f:
        credentials = google.oauth2.credentials.Credentials(token=None,
                                                            **json.load(f))

    with Assistant(credentials, args.device_model_id) as assistant:
        events = assistant.start()

        print('device_model_id:', args.device_model_id + '\n' +
              'device_id:', assistant.device_id + '\n')

        if args.project_id:
            register_device(args.project_id, credentials,
                            args.device_model_id, assistant.device_id)

        for event in events:
            process_event(event, assistant.device_id)
            

if __name__ == '__main__':
    main()
