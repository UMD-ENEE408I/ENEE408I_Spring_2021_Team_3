#!/usr/bin/env python3
# Derived from examples in the Flask-Ask repo: https://github.com/johnwheeler/flask-ask

from flask import Flask, g
from flask_ask import Ask, statement, session
import requests

app = Flask(__name__)
ask = Ask(app, '/')
stored_phrase = "you shouldn't see this"

def store_phrase(phrase):
    stored_phrase = phrase

def get_phrase():
    sess = requests.Session()
    sess.headers.update({'User-Agent': 'I am testing Alexa: Sentdex'})

#figured out how to store a phrase
#next thing is how to store it
#local variables don't work
#maybe something regarding flask session space?
@ask.intent('AddPhrase')
def add_phrase(phrase):
    stored_phrase = phrase #this sadly doesn't work, at least not between intents
    speech_text = "Stored Phrase: %s" % phrase
    return statement(speech_text)

@ask.intent('SayPhrase')
def say_phrase():
    #for now hard coded phrase in, in the future would replace with something like...
    #speech_text = stored_phrase (in whatever way we'd be storing it)
    #return(speech_text)
    speech_text = 'This is a test phrase for testing phrase storage'
    return statement(speech_text)

if __name__ == '__main__':
    app.run()
