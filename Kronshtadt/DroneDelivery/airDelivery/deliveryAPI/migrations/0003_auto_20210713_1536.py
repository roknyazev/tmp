# Generated by Django 3.2.5 on 2021-07-13 12:36

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('deliveryAPI', '0002_hub_hub_id'),
    ]

    operations = [
        migrations.AddField(
            model_name='bpla',
            name='departure_hub',
            field=models.IntegerField(blank=True, null=True),
        ),
        migrations.AddField(
            model_name='bpla',
            name='destination_hub',
            field=models.IntegerField(blank=True, null=True),
        ),
    ]
