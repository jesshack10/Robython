/**
 * @license
 * Visual Blocks Language
 *
 * Copyright 2012 Google Inc.
 * https://developers.google.com/blockly/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @fileoverview Helper functions for generating JavaScript for blocks.
 * @author fraser@google.com (Neil Fraser)
 */
'use strict';

goog.provide('Blockly.Robython');

goog.require('Blockly.Generator');


/**
 * JavaScript code generator.
 * @type {!Blockly.Generator}
 */
Blockly.Robython = new Blockly.Generator('Robython');

/**
 * List of illegal variable names.
 * This is not intended to be a security feature.  Blockly is 100% client-side,
 * so bypassing this list is trivial.  This is intended to prevent users from
 * accidentally clobbering a built-in object or function.
 * @private
 */
Blockly.Robython.addReservedWords(
    'Blockly,' +  // In case JS is evaled in the current window.
	'start,function,var,bool,text,num,true,false,print,if,else,for,while,'+
	//Palabra reservadas para el Robot
	'robot.getTemperature,robot.getDistance,robot.getRightLight,robot.getLeftLight,'+
	'robot.turnOnLED,robot.turnOffLED,robot.forward,robot.back,robot.left,robot.right,'+
	'robot.buzzer,robot.rgb,DO,DO+,RE,RE+,MI,MI+,FA,FA+,SOL,SOL+,LA,LA+,SI,SI+'
    );
/**
 * Order of operation ENUMs.
 * https://developer.mozilla.org/en/JavaScript/Reference/Operators/Operator_Precedence
 */
Blockly.JavaScript.ORDER_ATOMIC = 0;         // 0 "" ...
Blockly.JavaScript.ORDER_MEMBER = 1;         // . []
Blockly.JavaScript.ORDER_FUNCTION_CALL = 2;  // ()
Blockly.JavaScript.ORDER_BITWISE_NOT = 3;    // ~
Blockly.JavaScript.ORDER_UNARY_PLUS = 3;     // +
Blockly.JavaScript.ORDER_UNARY_NEGATION = 3; // -
Blockly.JavaScript.ORDER_MULTIPLICATION = 4; // *
Blockly.JavaScript.ORDER_DIVISION = 4;       // /
Blockly.JavaScript.ORDER_MODULUS = 4;        // %
Blockly.JavaScript.ORDER_ADDITION = 5;       // +
Blockly.JavaScript.ORDER_SUBTRACTION = 5;    // -
Blockly.JavaScript.ORDER_RELATIONAL = 6;     // < <= > >=
Blockly.JavaScript.ORDER_EQUALITY = 7;       // == != === !==
Blockly.JavaScript.ORDER_LOGICAL_AND = 8;   // &&
Blockly.JavaScript.ORDER_LOGICAL_OR = 9;    // ||
Blockly.JavaScript.ORDER_COMMA = 10;         // ,
Blockly.JavaScript.ORDER_NONE = 99;          // (...)

/**
 * Initialise the database of variable names.
 * @param {!Blockly.Workspace} workspace Workspace to generate code from.
 */
Blockly.Robython.init = function(workspace) {
  // Create a dictionary of definitions to be printed before the code.
  Blockly.Robython.definitions_ = Object.create(null);
  // Create a dictionary mapping desired function names in definitions_
  // to actual function names (to avoid collisions with user functions).
  Blockly.Robython.functionNames_ = Object.create(null);

  if (!Blockly.Robython.variableDB_) {
    Blockly.Robython.variableDB_ =
        new Blockly.Names(Blockly.Robython.RESERVED_WORDS_);
  } else {
    Blockly.Robython.variableDB_.reset();
  }

  var defvars = [];
  var variables = Robython.Variables.allVariables(workspace);
  for (var i = 0; i < variables.length; i++) {
    defvars[i] = 'var ' +
        Blockly.Robython.variableDB_.getName(variables[i],
        Blockly.Variables.NAME_TYPE) + ';';
  }
  Blockly.Robython.definitions_['variables'] = defvars.join('\n');
};

/**
 * Prepend the generated code with the variable definitions.
 * @param {string} code Generated code.
 * @return {string} Completed code.
 */
Blockly.Robython.finish = function(code) {
  // Convert the definitions dictionary into a list.
  var definitions = [];
  for (var name in Blockly.Robython.definitions_) {
    definitions.push(Blockly.Robython.definitions_[name]);
  }
  // Clean up temporary data.
  delete Blockly.Robython.definitions_;
  delete Blockly.Robython.functionNames_;
  Blockly.Robython.variableDB_.reset();
  return definitions.join('\n\n') + '\n\n\n' + code;
};

/**
 * Naked values are top-level blocks with outputs that aren't plugged into
 * anything.  A trailing semicolon is needed to make this legal.
 * @param {string} line Line of generated code.
 * @return {string} Legal line of code.
 */
Blockly.Robython.scrubNakedValue = function(line) {
  return line + ';\n';
};

/**
 * Encode a string as a properly escaped JavaScript string, complete with
 * quotes.
 * @param {string} string Text to encode.
 * @return {string} JavaScript string.
 * @private
 */
Blockly.Robython.quote_ = function(string) {
  // TODO: This is a quick hack.  Replace with goog.string.quote
  string = string.replace(/\\/g, '\\\\')
                 .replace(/\n/g, '\\\n')
                 .replace(/'/g, '\\\'');
  return '\'' + string + '\'';
};

/**
 * Common tasks for generating JavaScript from blocks.
 * Handles comments for the specified block and any connected value blocks.
 * Calls any statements following this block.
 * @param {!Blockly.Block} block The current block.
 * @param {string} code The JavaScript code created for this block.
 * @return {string} JavaScript code with comments and subsequent blocks added.
 * @private
 */
Blockly.Robython.scrub_ = function(block, code) {
  var commentCode = '';
  // Only collect comments for blocks that aren't inline.
  if (!block.outputConnection || !block.outputConnection.targetConnection) {
    // Collect comment for this block.
    var comment = block.getCommentText();
    if (comment) {
      commentCode += Blockly.Robython.prefixLines(comment, '// ') + '\n';
    }
    // Collect comments for all value arguments.
    // Don't collect comments for nested statements.
    for (var x = 0; x < block.inputList.length; x++) {
      if (block.inputList[x].type == Blockly.INPUT_VALUE) {
        var childBlock = block.inputList[x].connection.targetBlock();
        if (childBlock) {
          var comment = Blockly.Robython.allNestedComments(childBlock);
          if (comment) {
            commentCode += Blockly.Robython.prefixLines(comment, '// ');
          }
        }
      }
    }
  }
  var nextBlock = block.nextConnection && block.nextConnection.targetBlock();
  var nextCode = Blockly.Robython.blockToCode(nextBlock);
  return commentCode + code + nextCode;
};
