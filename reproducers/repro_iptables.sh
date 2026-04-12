#!/bin/sh
export LD_LIBRARY_PATH=/host_tools
export XTABLES_LIBDIR=/usr/lib/x86_64-linux-gnu/xtables
LO="/host_tools/ld-linux-x86-64.so.2"
IPTABLES="$LO /host_tools/iptables"
PING="$LO /host_tools/ping"

$LO /sbin/ifconfig lo 127.0.0.1 up

# Clear old rules
$IPTABLES -F

echo "--- Baseline test ---"
$PING -c 1 127.0.0.1 > /dev/null 2>&1
if [ $? -eq 0 ]; then echo "Baseline SUCCESS"; else echo "Baseline FAILED"; fi

echo "Setting up iptables rule (Match ICMP Type 8)..."
# Pattern: Specific (0), Single (0), Value 8
# Hex: |00000008|
$IPTABLES -A INPUT -p icmp -m string --hex-string "|00000008|" --algo fsm --from 20 --to 25 -j DROP

echo "Test 1: Match anywhere in window (Should DROP)"
$PING -c 1 127.0.0.1 > /dev/null 2>&1
if [ $? -eq 0 ]; then echo "FAILED: Ping passed (Bug still present)"; else echo "SUCCESS: Ping dropped (Fix confirmed!)"; fi

echo "--- Iptables stats ---"
$IPTABLES -L INPUT -v -n
