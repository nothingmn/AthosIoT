<template>
    <modal name="rename-relay-modal"  @before-open="beforeOpen">
      <card>
        <h4 slot="header" class="card-title">Rename Relay</h4>
        <div class="row">
          <div class="col-lg-12">
            Please enter the new name for the pin:{{relay.id}}
          </div>
        </div>
        <div class="row">
          <div class="col-lg-12">
            <input v-model="relay.name" placeholder="Relay Name">
          </div>
        </div>
        <div class="row">
          <div class="col-lg-12">
            <button type="submit" class="btn btn-secondary  float-right" @click="hide()">
            Cancel
            </button>
            <button type="submit" class="btn btn-secondary  float-right" @click="relay_rename()">
            Save
            </button>

          </div>
      </div>
      </card>
    </modal>
</template>
<script>
import Card from 'src/components/Cards/Card.vue'
import Vue from 'vue'

export default {
  components: {
      Card
  },  
  name: 'rename-relay-modal',  
  //props : ['relay', 'device', 'existingName'],
  data () {
    return {
      relay : {},
      device : {}
    }
  },
  methods: {
    beforeOpen (event) {
      this.relay = event.params.model.relay;
      this.device = event.params.model.device;
    },
    hide() {
        this.$modal.hide('rename-relay-modal');
    },
    relay_rename(){
          console.log('renaming relay switch:', this.relay.id, this.relay.name );
          Vue.prototype.$socket.send(JSON.stringify({ action : "rename-relay-switch", device : this.device, relay : this.relay, name: this.relay.name }));
          this.$modal.hide('rename-relay-modal');

        },
  }
}
</script>