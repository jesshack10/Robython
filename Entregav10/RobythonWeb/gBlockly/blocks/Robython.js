'use strict';
goog.require('Blockly.Blocks');
/*
*This creates a RGB block for Robython
*/
Blockly.Blocks['robython_rgb'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("ROBYTHON");
    this.appendValueInput("R")
        .setCheck("Number")
        .appendField("R");
    this.appendValueInput("G")
        .setCheck("Number")
        .appendField("G");
    this.appendValueInput("B")
        .setCheck("Number")
        .appendField("B");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setColour(120);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};
/*
*This blocks allows us to manipulate the Buzzer 
*of the Robot
*/

Blockly.Blocks['robython_buzzer'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("ROBYTHON BUZZER");
    this.appendValueInput("time")
        .setCheck("Number")
        .appendField(new Blockly.FieldDropdown([["DO", "C"], ["RE", "D"], ["MI", "E"], ["FA", "F"], ["SOL", "G"], ["LA", "A"], ["SI", "B"], ["DO+", "C+"]]), "Notes");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setColour(210);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};
/*
*This blocks was designed for movements of the Robot
*/
Blockly.Blocks['robython_move'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("ROBYTHON  MOVE")
        .appendField(new Blockly.FieldDropdown([["FORWARD", "rf"], ["BACKWARD", "rb"], ["RIGHT", "rr"], ["LEFT", "rl"]]), "robython_move");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setColour(180);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};
/*
*This block is to turn on/off LEDs in the Robot
*/
Blockly.Blocks['robython_onoffled'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("ROBYTHON LED CONTROL ON/OFF");
    this.appendDummyInput()
        .appendField("LED1")
        .appendField(new Blockly.FieldCheckbox("FALSE"), "LED1")
        .appendField("LED2")
        .appendField(new Blockly.FieldCheckbox("FALSE"), "LED2")
        .appendField("LED3")
        .appendField(new Blockly.FieldCheckbox("FALSE"), "LED3")
        .appendField("LED4")
        .appendField(new Blockly.FieldCheckbox("FALSE"), "LED4");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setColour(135);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};
/*
*This block allows to get temperature from Robot
*/
Blockly.Blocks['robython_temp'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("ROBYTHON TEMPERATURE");
    this.setOutput(true, "Number");
    this.setColour(180);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};
/*
*This block allows to get distance from Robot
*/
Blockly.Blocks['robython_distance'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("ROBYTHON DISTANCE");
    this.setOutput(true, "Number");
    this.setColour(180);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};
/*
*This blocks allows to get light from any side of the robot
*/
Blockly.Blocks['robython_light'] = {
  init: function() {
    this.appendDummyInput()
        .setAlign(Blockly.ALIGN_CENTRE)
        .appendField("ROBYTHON LIGHT");
    this.appendDummyInput()
        .setAlign(Blockly.ALIGN_CENTRE)
        .appendField(new Blockly.FieldDropdown([["LEFT", "L"], ["RIGHT", "R"]]), "NAME");
    this.setInputsInline(false);
    this.setOutput(true, "Number");
    this.setColour(180);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};