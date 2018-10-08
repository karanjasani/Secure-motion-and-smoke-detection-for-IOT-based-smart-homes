import time
import serial
import smtplib
TO = '' -- Enter To email here 
GMAIL_USER = '' -- Enter From email here
GMAIL_PASS = '' -- Enter password of From email
SUBJECT = 'Intrusion!!'
TEXT = 'Your PIR sensor detected movement'
SUBJECT_GAS = 'Smoke!!'
TEXT_GAS = 'Your Gas sensor detected smoke'


def send_email_pir():
    print("Sending Email")
    smtpserver = smtplib.SMTP("smtp.gmail.com",587)
    smtpserver.ehlo()
    smtpserver.starttls()
    smtpserver.ehlo
    smtpserver.login(GMAIL_USER, GMAIL_PASS)
    header = 'To:' + TO + '\n' + 'From: ' + GMAIL_USER
    header = header + '\n' + 'Subject:' + SUBJECT + '\n'
    print header
    msg = header + '\n' + TEXT + ' \n\n'
    smtpserver.sendmail(GMAIL_USER, TO, msg)
    smtpserver.close()

def send_email_gas():
    print("Sending Email")
    smtpserver = smtplib.SMTP("smtp.gmail.com",587)
    smtpserver.ehlo()
    smtpserver.starttls()
    smtpserver.ehlo
    smtpserver.login(GMAIL_USER, GMAIL_PASS)
    header = 'To:' + TO + '\n' + 'From: ' + GMAIL_USER
    header = header + '\n' + 'Subject:' + SUBJECT_GAS + '\n'
    print header
    msg = header + '\n' + TEXT_GAS + ' \n\n'
    smtpserver.sendmail(GMAIL_USER, TO, msg)
    smtpserver.close()

ser = serial.Serial('COM4', 115200)

while True:
    x = ser.readline()

    if x[0] == 'A':
        while True:
            message = ser.readline()
            print(message)
            if message[0] == 'm' :
                send_email_pir()
                break
    if x[0] == 's':
        send_email_gas()
        

#while True:
 #   message = ser.readline()
  #  print(message)
   # if message[0] == 'M' :
    #    send_email()
    #time.sleep(0.5)

