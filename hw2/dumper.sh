./arger-yu -u Help Is on THE way
if [ $? -eq 0 ]; then
    echo "arger-yu upper-case ran successfully"
else
    echo "arger-yu upper-case error: improper usage"
fi

./arger-yu -l What are 2 Companies wherE you Want To Work?
if [ $? -eq 0 ]; then
    echo "arger-yu lower-case ran successfully"
else
    echo "arger-yu lower-case error: improper usage"
fi

./arger-yu -cap bESt lEssOn I Ever took
if [ $? -eq 0 ]; then
    echo "arger-yu capital-case ran successfully"
else
    echo "arger-yu capital-case error: improper usage"
fi

./arger-yu -c bESt lEssOn I Ever took
if [ $? -eq 0 ]; then
    echo "arger-yu capital-case ran successfully"
else
    echo "arger-yu capital-case error: improper usage"
fi

exit 0