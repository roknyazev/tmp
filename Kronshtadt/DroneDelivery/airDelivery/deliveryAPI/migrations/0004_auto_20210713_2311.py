# Generated by Django 3.2.5 on 2021-07-13 20:11

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('deliveryAPI', '0003_auto_20210713_1536'),
    ]

    operations = [
        migrations.AddField(
            model_name='bpla',
            name='lat_delta',
            field=models.IntegerField(blank=True, null=True),
        ),
        migrations.AddField(
            model_name='bpla',
            name='long_delta',
            field=models.IntegerField(blank=True, null=True),
        ),
    ]
