#!/usr/local/bin/perl

use strict;
use warnings;


use Device::SerialPort;



my $payload = "ABC";




my $devicelabel = $ARGV[0];
my $deviceport = $ARGV[1];

if ( not defined( $devicelabel ) ) { die( "No label specified\n" ); }
$devicelabel = $devicelabel . ": ";

if ( not defined( $deviceport ) ) { die( "No port specified\n" ); }


my $serialporthandle = Device::SerialPort->new( $deviceport );

if ( not defined( $serialporthandle ) ) {
    die( "Could not open serial port located at:" . $deviceport . "\n" );
}

$serialporthandle->baudrate(300);
$serialporthandle->databits(8);
$serialporthandle->parity("none");
$serialporthandle->stopbits(1);

print( $devicelabel . "Port configured: " . $deviceport , "\n" );


my $incomingdata = "";
while (1) {
    $incomingdata = $serialporthandle->read(255);
    if ( $incomingdata ne "") {
    print( $devicelabel . "recv: " . $incomingdata . "\n" );
    $incomingdata = "";
    }
}



