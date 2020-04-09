<template>
  <card>
    <h4 slot="header" class="card-title">{{device.name}} <div class="float-right small">({{device.deviceid}} - {{device.type}} - {{device.version}})</div></h4>    
    <hr width="80%" />
    <div class="row">
      <div class="col-lg-12">
        <button type="submit" class="btn btn-secondary float-right" @click.prevent="wipe">
          Wipe
        </button>
        <button type="submit" class="btn btn-light float-right" @click.prevent="reset">
          Reset
        </button>
        <button type="submit" class="btn btn-secondary  float-right" @click.prevent="rename(device)">
          Rename
        </button>
        <button type="submit" class="btn btn-secondary float-right" @click.prevent="ping">
          Ping
        </button>        
        <button type="submit" class="btn btn-secondary float-right" @click.prevent="firmware_update(device)">
          Firmware
        </button>        
        <div v-if="device.last.type == 'RELAY'">
            <button type="submit" class="btn btn-secondary  float-right" @click.prevent="all_off(device)">
              All Off
            </button>
            <button type="submit" class="btn btn-secondary  float-right" @click.prevent="all_on(device)">
              All On
            </button>
        </div>
      </div>
    </div>
    <div class="row">
      <div class="col-md-12">
        <div v-if="device.last">
          <div v-if="device.last.type == 'BMP280'">
            <div class="table-responsive col-md-12">
              <table class="table table-hover">
                  <tbody>
                    <tr>
                      <th scope="row">Temperature</th>
                      <td>{{device.last.temp}}&deg;C</td>
                    </tr>
                    <tr>
                      <th scope="row">Humidity</th>
                      <td>{{device.last.humidity}}</td>
                    </tr>
                    <tr>
                      <th scope="row">Pressure</th>
                      <td>{{device.last.pressure}}</td>
                    </tr>
                    <tr>
                      <th scope="row">Altitude</th>
                      <td>{{device.last.altitude}}</td>
                    </tr>
                    <tr>
                      <th scope="row">Ping Delay</th>
                      <td>
                        <div v-if="device.ping">{{device.ping.roundTrip}}</div></td>
                    </tr>
                    <tr>
                      <th scope="row">Last Updated</th>
                      <td>{{device.last.timeStamp}}</td>
                    </tr>
                  </tbody>
              </table>
            </div>
          </div>          
          <div v-if="device.last.type == 'TMP36'">
            <div class="table-responsive col-md-12">
              <table class="table table-hover">
                  <tbody>
                    <tr>
                      <th scope="row">Temperature</th>
                      <td>{{device.last.temp}}&deg;C</td>
                    </tr>                  
                    <tr>
                      <th scope="row">Ping Delay</th>
                      <td>
                        <div v-if="device.ping">{{device.ping.roundTrip}}</div></td>
                    </tr>
                    <tr>
                      <th scope="row">Last Updated</th>
                      <td>{{device.last.timeStamp}}</td>
                    </tr>
                  </tbody>
              </table>
            </div>          </div>          
          <div v-if="device.last.type == 'DHT11'">
            <div class="table-responsive col-md-12">
              <table class="table table-hover">
                  <tbody>
                    <tr>
                      <th scope="row">Temperature</th>
                      <td>{{device.last.temp}}&deg;C</td>
                    </tr>
                    <tr>
                      <th scope="row">Humidity</th>
                      <td>{{device.last.humidity}}</td>
                    </tr>
                    <tr>
                      <th scope="row">Heat Index</th>
                      <td>{{device.last.headIndex}}</td>
                    </tr>
                    <tr>
                      <th scope="row">Ping Delay</th>
                      <td>
                        <div v-if="device.ping">{{device.ping.roundTrip}}</div></td>
                    </tr>
                    <tr>
                      <th scope="row">Last Updated</th>
                      <td>{{device.last.timeStamp}}</td>
                    </tr>
                  </tbody>
              </table>
            </div>            
          </div>          
          <div v-if="device.last.type == 'PIR'">

            <div class="table-responsive col-md-12">
              <table class="table table-hover">
                  <tbody>
                    <tr>
                      <th scope="row">Movement</th>
                      <td>{{device.last_motion.timeStamp}}</td>
                    </tr>
                    <tr>
                      <th scope="row">State</th>
                      <td>{{device.last_motion.movement}}</td>
                    </tr>
                    <tr>
                      <th scope="row">Ping Delay</th>
                      <td>
                        <div v-if="device.ping">{{device.ping.roundTrip}}</div></td>
                    </tr>
                    <tr>
                      <th scope="row">Last Updated</th>
                      <td>{{device.last.timeStamp}}</td>
                    </tr>
                  </tbody>
              </table>
            </div>               
          </div>          
          <div v-if="device.last.type == 'RELAY'">
            <ul list-group class="list-group">
              <li v-for="o in device.relay" class="list-group-item list-group-item-action flex-column align-items-start">
                <div v-if="o.id">
                  {{o.id}} : {{o.name}}
                  <button type="submit" class="btn btn-secondary  float-right" @click="relay_rename(device, o)">
                    Rename
                  </button>
                  <button type="submit" class="btn btn-secondary  float-right" @click.prevent="relay_off(device, o)">
                    Off
                  </button>
                  <button type="submit" class="btn btn-secondary  float-right" @click.prevent="relay_on(device, o)">
                    On
                  </button>
                </div>          
              </li>
            </ul>
          </div>          
        </div>          
      </div>
    </div>
    <div class="clearfix"></div>
    <rename-relay-modal name="rename-relay-modal"/>
    <rename-device-modal name="rename-device-modal"/>
    <device-firmware-update-modal name="device-firmware-update-modal"/>

  </card>  

</template>
<script>
import Card from 'src/components/Cards/Card.vue'
import Vue from 'vue'
import RenameRelayModal from 'src/components/RenameRelayModal.vue'
import RenameDeviceModal from 'src/components/RenameDeviceModal.vue'
import DeviceFirmwareUpdateModal from 'src/components/DeviceFirmwareUpdateModal.vue'

export default {
    components: {
      Card,
      RenameRelayModal,
      RenameDeviceModal,
      DeviceFirmwareUpdateModal
    },
    props : ['device'],
    data () {
      return {
      }
    },
      mounted() {    
      },   
      methods: {
        all_on(device) {
          console.log('all_on:',  device.deviceid);
          Vue.prototype.$socket.send(JSON.stringify({ action : "all-on", deviceid : device.deviceid }));

        },
        all_off(device) {
          console.log('all_off:',  device.deviceid);
          Vue.prototype.$socket.send(JSON.stringify({ action : "all-off", deviceid : device.deviceid }));
        },
        firmware_update(device) {
          console.log('firmware_update:',  device.deviceid);

          this.$modal.show('device-firmware-update-modal', { model : {
            device : device
          }});
        },
        relay_rename(device, relay){
          console.log('rename relay:',  device.deviceid, relay.id);

          this.$modal.show('rename-relay-modal', { model : {
            relay : relay,
            device : device,
          }});
        },
        relay_on(device, relay){
          console.log('on relay:',  device.deviceid, relay.id);
          Vue.prototype.$socket.send(JSON.stringify({ action : "relay-on", deviceid : device.deviceid, relay : relay.id }));
        },
        relay_off(device, relay){
          console.log('off relay:',  device.deviceid, relay.id);
          Vue.prototype.$socket.send(JSON.stringify({ action : "relay-off", deviceid : device.deviceid, relay : relay.id }));
        },
        ping() {
          console.log('pinging device:', this.device.deviceid );
          Vue.prototype.$socket.send(JSON.stringify({ action : "ping-device", ts: new Date(), deviceid : this.device.deviceid }));
        }, 
        rename(device) {
          console.log('rename device:', this.device.deviceid );
          this.$modal.show('rename-device-modal', { model : { device : device }});
        }, 
        wipe() {
          console.log('wiping device:', this.device.deviceid );
          Vue.prototype.$socket.send(JSON.stringify({ action : "wipe-device", deviceid : this.device.deviceid }));
        }, 
        reset() {
          console.log('reset device:', this.device.deviceid );
          Vue.prototype.$socket.send(JSON.stringify({ action : "reset-device", deviceid : this.device.deviceid }));
        },
        reboot() {
          console.log('reboot device:', this.device.deviceid );
          Vue.prototype.$socket.send(JSON.stringify({ action : "reboot-device", deviceid : this.device.deviceid }));
        }
    }
  }

</script>
<style>

</style>
