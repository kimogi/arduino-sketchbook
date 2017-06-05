#!/bin/bash

usage() { echo "Usage: $0 [-d <device>] [-f <file>]" 1>&2; exit 1; }

CMD=""
FILE=""

while getopts ":d:f:" o; do
    case "${o}" in
        d)
            CMD="$CMD --port ${OPTARG}"
            ;;
        f)
            FILE=${OPTARG}
            ;;
        *)
            usage
            ;;
    esac
done

[[ $CMD ]] || usage

arduino --board Microduino:avr:644pa16m $CMD --upload $FILE
