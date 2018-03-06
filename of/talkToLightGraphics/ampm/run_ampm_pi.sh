#!/bin/bash
#add following to /home/pi/.config/lxsession/LXDE-pi
#--> @xterm -e "$HOME/rph/talktolightdev/of/talkToLightGraphics/ampm/run_ampm_pi.sh"
#make sure path is to this file

echo "launching ampm + google assistant"
#xterm -hold -e "make run -C rph/talktolightdev/of/talkToLightGraphics/" &
#make run -C $HOME/rph/talktolightdev/of/talkToLightGraphics/ &
cd $HOME/rph/talktolightdev/of/talkToLightGraphics/ampm
ampm ./ampm.json &
xterm -hold -e "source $HOME/rph/env/bin/activate && python $HOME/rph/talktolightdev/py/talktolight.py --device_model_id talk-to-light-model-v1"
