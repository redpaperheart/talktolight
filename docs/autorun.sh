#!/bin/bash

xterm -hold -e "source $HOME/rph/env/bin/activate && python $HOME/rph/talktolight/py/talktolight.py --device_model_id talk-to-light-v1" &

sleep 5

cd $HOME/rph/talktolight/of/talkToLightGraphics/
make run
