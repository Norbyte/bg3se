aws s3 cp PublishingRoot\Manifest2.json s3://nb-stor/bg3-updater-v2/Channels/Devel/Manifest2.json
aws s3api put-object-acl --bucket nb-stor --key bg3-updater-v2/Channels/Devel/Manifest2.json --acl public-read
aws cloudfront create-invalidation --distribution-id E300IVONTDEMJX --paths /Channels/Devel/Manifest2.json
pause
